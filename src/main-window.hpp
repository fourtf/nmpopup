#pragma once

#include <QMainWindow>
#include <QTime>

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

  QTime creationTime_; // used to delay keyboard enter input
  NetworkTableModel *model_{};
  QTableView *wifiList_{};
};
