#pragma once

#include <QString>

class Theme {
public:
  Theme();

  void loadInternalDarkTheme();
  void installDarkTheme();
  void installInternalStyle(const QString &name);

  QString qss;
};

Theme &theme();
