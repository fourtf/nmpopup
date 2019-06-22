#pragma once

class QStringList;

enum class Command {
  Run,
  ShowHelp,
};

class Config {
public:
  Config();

  void parse(const QStringList &args);

  Command command = {};
  bool borderless = false;
  bool showDuplicates = false;
  bool darkPalette = true;
};

Config &config();
