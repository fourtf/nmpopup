#include <QApplication>
#include <QDebug>
#include <QFile>

#include "config.hpp"
#include "main-window.hpp"
#include "networking.hpp"
#include "password-dialog.hpp"
#include "theme.hpp"

int main(int argc, char *argv[]) {
  QApplication::setApplicationName("nmpopup");

  // parse command line
  auto args = QStringList();
  std::transform(argv + 1, argv + argc, std::back_inserter(args),
                 [&](auto s) { return s; });
  config().parse(args);

  // execute different commands
  switch (config().command) {
    case Command::Run: {
      QApplication a(argc, argv);

      if (config().darkPalette) {
        theme().installDarkTheme();
        theme().loadInternalDarkTheme();
      }

      // password dialog
      QObject::connect(&networking(), &Networking::passwordRequired,
                       [](const QString &ssid) {
                         showPasswordDialog(ssid, [](const QString &ssid,
                                                     const QString &password) {
                           config().setPasswordForSsid(ssid, password);
                           networking().connectWifi(ssid, password);
                         });
                       });

      MainWindow w;
      w.show();
      return a.exec();
    } // break;
    case Command::ShowHelp: {
      QFile help(":/help.txt");
      help.open(QIODevice::ReadOnly);
      qInfo().noquote() << QString(help.readAll());
    } break;
  }
}
