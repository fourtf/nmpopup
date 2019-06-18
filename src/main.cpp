#include <QApplication>

#include "config.hpp"
#include "main-window.hpp"
#include "networking.hpp"
#include "theme.hpp"

#include <QMessageBox>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // parse command line
  auto args = QStringList();
  std::transform(argv + 1, argv + argc, std::back_inserter(args),
                 [&](auto s) { return s; });
  config().parse(args);

  // theme().installInternalStyle("Windows");
  theme().installDarkTheme();
  theme().loadInternalDarkTheme();

  MainWindow w;
  w.show();

  return a.exec();
}
