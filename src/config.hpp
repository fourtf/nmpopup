#pragma once

#include <QSettings>

class QStringList;

enum class Command {
  Run,
  ShowHelp,
};

class Config {
public:
  Config();

  void parse(const QStringList &args);
  QString getPasswordForSsid(const QString &ssid);
  void setPasswordForSsid(const QString &ssid, const QString &password);

  Command command = {};
  bool borderless = false;
  bool showDuplicates = false;
  bool darkPalette = true;

private:
  QString configBasePath;
  QSettings ssids;
};

Config &config();
