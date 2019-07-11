#include "config.hpp"

#include <QDebug>
#include <QStandardPaths>
#include <QStringList>
#include <iostream>

Config::Config()
    : configBasePath(QStandardPaths::standardLocations(
          QStandardPaths::AppConfigLocation)[0]),
      ssids(this->configBasePath + "/config", QSettings::Format::IniFormat) {

  qInfo() << "ssid save location (plain text): " << this->configBasePath;
}

Config &config() {
  static Config instance;
  return instance;
}

void Config::parse(const QStringList &args) {
  // call next() to get the next argument
  auto next = [&, i = 0]() mutable -> QString {
    return i < args.size() ? args[i++] : QString();
  };

  QString arg;
  while (!(arg = next()).isNull()) {
    if (arg == "--help" || arg == "-help" || arg == "?") {
      command = Command::ShowHelp;
    } else if (arg == "--duplicates") {
      this->showDuplicates = true;
    } else if (arg == "--borderless") {
      this->borderless = true;
    } else if (arg == "--preserve-style") {
      this->darkPalette = false;
    } else if (arg.startsWith("--")) {
      qWarning() << "unknown argument " << arg;
    }
    // We skip commands that start with "-" since they might be qt arguments.
  }
}

QString Config::getPasswordForSsid(const QString &ssid) {
  return this->ssids.value("ssids/" + ssid).toString();
}

void Config::setPasswordForSsid(const QString &ssid, const QString &password) {
  this->ssids.setValue("ssids/" + ssid, password);
}
