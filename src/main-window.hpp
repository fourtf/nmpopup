#pragma once

#include <QMainWindow>

class NetworkTableModel;
class QTableView;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

  void keyReleaseEvent(QKeyEvent *) override;

private:
  void initLayout();
  QString selectedWifi();

  NetworkTableModel *model_{};
  QTableView *wifiList_{};
};
