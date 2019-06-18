#pragma once

#include <QObject>
#include <QString>
#include <vector>

struct WifiNetwork {
  bool inUse;
  QString ssid;
  QString mode;
  QString channel;
  QString rate;
  QString signal;
  QString bars;
  QString security;
};

class Networking : public QObject {
  Q_OBJECT

public:
  Networking();
  virtual ~Networking() = default;

  // QString wifiDevice();
  void queryWifiNetworks(bool rescan = true);
  void connectWifi(const QString &ssid, const QString &password = {});

signals:
  void wifiNetworksUpdated(const std::vector<WifiNetwork> &);

private:
  bool queryingNetworks_{};
};

Networking &networking();
