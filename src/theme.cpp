#include "theme.hpp"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QPalette>
#include <QStyleFactory>

Theme::Theme() {}

Theme &theme() {
  static Theme instance;

  return instance;
}

void Theme::loadInternalDarkTheme() {
  QFile file(":/dark.qss");
  file.open(QIODevice::ReadOnly);
  this->qss = QString(file.readAll());
}

void Theme::installDarkTheme() {
  // borrowed from
  // https://stackoverflow.com/questions/15035767/is-the-qt-5-dark-fusion-theme-available-for-windows
  auto dark = qApp->palette();

  dark.setColor(QPalette::Window, QColor(22, 22, 22));
  dark.setColor(QPalette::WindowText, Qt::white);
  dark.setColor(QPalette::Text, Qt::white);
  dark.setColor(QPalette::Disabled, QPalette::WindowText,
                QColor(127, 127, 127));
  dark.setColor(QPalette::Base, QColor("#222"));
  dark.setColor(QPalette::AlternateBase, QColor("#333"));
  dark.setColor(QPalette::ToolTipBase, Qt::white);
  dark.setColor(QPalette::ToolTipText, Qt::white);
  dark.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
  dark.setColor(QPalette::Dark, QColor(35, 35, 35));
  dark.setColor(QPalette::Shadow, QColor(20, 20, 20));
  dark.setColor(QPalette::Button, QColor(70, 70, 70));
  dark.setColor(QPalette::ButtonText, Qt::white);
  dark.setColor(QPalette::Disabled, QPalette::ButtonText,
                QColor(127, 127, 127));
  dark.setColor(QPalette::BrightText, Qt::red);
  dark.setColor(QPalette::Link, QColor(42, 130, 218));
  dark.setColor(QPalette::Highlight, QColor(42, 130, 218));
  dark.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
  dark.setColor(QPalette::HighlightedText, Qt::white);
  dark.setColor(QPalette::Disabled, QPalette::HighlightedText,
                QColor(127, 127, 127));

  qApp->setPalette(dark);
}

void Theme::installInternalStyle(const QString &name) {
  QApplication::setStyle(QStyleFactory::create(name));
  // qDebug() << QStyleFactory::keys();
}
