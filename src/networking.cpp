#include "networking.hpp"

#include <QDebug>
#include <QProcess>
#include <QThread>
#include <unordered_map>
#include <vector>

#include "config.hpp"
#include "util.hpp"

namespace {

class CliOutput {
  std::vector<std::vector<QString>> rows_;

public:
  QString raw;

  CliOutput(const QString &value) : raw(value) {
    // parse terse escaped string
    std::vector<QString> row;
    QString current;

    for (int i = 0; i < value.length(); i++) {
      if (value[i] == '\\' && i + 1 < value.length()) {
        i++;
        current += value[i];
      } else if (value[i] == ':') {
        row.push_back(current);
        current = "";
      } else if (value[i] == '\n') {
        if (!current.isNull()) {
          row.push_back(current);
        }
        this->rows_.push_back(std::move(row));
      } else {
        current += value[i];
      }
    }
    if (!current.isNull()) {
      row.push_back(current);
    }
    if (!row.empty()) {
      this->rows_.push_back(std::move(row));
    }
  }

  std::vector<QString> column(unsigned index) const {
    std::vector<QString> result;

    for (auto &&row : this->rows_) {
      if (row.size() > index) {
        result.push_back(row[index]);
      }
    }

    return result;
  }

  const std::vector<std::vector<QString>> rows() const { return this->rows_; }
};

CliOutput runCli(const QStringList &params) {
  QProcess process;
  process.start("nmcli", params);
  process.waitForFinished();
  auto arr = process.readAllStandardOutput();
  auto str = QString(arr);

  return {str};
}

std::vector<WifiNetwork> parseWifiNetworks(QByteArray arr) {
  auto output = CliOutput(QString(arr));
  std::vector<WifiNetwork> result;

  QSet<QString> ssids;

  for (auto &&row : output.rows()) {
    continue_if(row.size() < 8);

    if (!config().showDuplicates) {
      if (ssids.contains(row[1])) {
        continue;
      }
      ssids.insert(row[1]);
    }

    // IN-USE SSID MODE CHAN RATE SIGNAL BARS SECURITY
    result.push_back({row[0] == "*", row[1], row[2], row[3], row[4], row[5],
                      row[6], row[7]});
  }

  return result;
}

} // namespace

Networking::Networking() {}

/*
 QString Networking::wifiDevice() {
  auto output = runCli({"-t", "device"});

  for (auto &&row : output.rows()) {
    continue_if(row.size() < 3);

    if (row[1] == "wifi" && row[2] == "connected") {
      return row[0];
    }
  }

  return {};
}*/

void Networking::queryWifiNetworks(bool rescan) {
  return_if(rescan && this->queryingNetworks_);

  auto process = new QProcess;

  QObject::connect(process, qOverload<int>(&QProcess::finished), this,
                   [this, process](int) {
                     auto arr = process->readAllStandardOutput();

                     emit this->wifiNetworksUpdated(parseWifiNetworks(arr));
                     process->deleteLater();
                   });

  process->start("nmcli", {"-t", "device", "wifi", "list", "-rescan",
                           rescan ? "yes" : "no"});
  this->queryingNetworks_ |= rescan;
}

void Networking::connectWifi(const QString &ssid, const QString &password) {
  QProcess::startDetached("nmcli", {"device", "wifi", "connect", ssid});
}

Networking &networking() {
  static Networking instance;

  return instance;
}
