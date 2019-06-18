#include "config.hpp"

#include <QDebug>
#include <QStringList>
#include <iostream>

Config::Config() {}

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
    if (arg == "--duplicates") {
      this->showDuplicates = true;
    } else if (arg == "--borderless") {
      this->borderless = true;
    } else {
      qWarning() << "unknown argument " << arg;
    }
  }
}
