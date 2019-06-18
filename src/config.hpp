#pragma once

class QStringList;

class Config {
public:
  Config();

  void parse(const QStringList &args);

  bool borderless = false;
  bool showDuplicates = false;
};

Config &config();
