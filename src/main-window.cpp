#include "main-window.hpp"

#include "config.hpp"
#include "function-event-filter.hpp"
#include "make-widget.hpp"
#include "network-table-model.hpp"
#include "networking.hpp"
#include "theme.hpp"
#include "util.hpp"

#include <QHeaderView>
#include <QKeyEvent>
#include <QPushButton>
#include <QStringListModel>
#include <QTableView>
#include <QThread>

#include <QDebug>

namespace {
  constexpr int ssidColumn = 1;

  QString selectedItem(QTableView *view) {
    return view->currentIndex()
        .siblingAtColumn(1)
        .data(Qt::DisplayRole)
        .toString();
  }

  void selectItem(QTableView *view, const QString &selected) {
    // keep selection
    if (selected.isNull()) {
      if (view->model()->rowCount() > 0) {
        view->setCurrentIndex(view->model()->index(0, 0));
      }
    } else {
      for (int i = 0; i < view->model()->rowCount(); i++) {
        auto index = view->model()->index(i, ssidColumn);
        if (selected == index.data(Qt::DisplayRole).toString()) {
          view->setCurrentIndex(index);
          break;
        }
      }
    }
  }

  auto windowType() {
    unsigned type = Qt::Dialog;
    if (config().borderless) {
      type |= Qt::FramelessWindowHint;
    }
    return Qt::WindowFlags(type);
  }
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, ::windowType()), creationTime_(QTime::currentTime()) {
  this->model_ = new NetworkTableModel();

  QObject::connect(&networking(), &Networking::wifiNetworksUpdated, this,
                   [this](auto &&items) {
                     auto selected = selectedItem(this->wifiList_);

                     this->model_->setItems(items);
                     this->wifiList_->resizeColumnsToContents();

                     selectItem(this->wifiList_, selected);
                   });

  networking().queryWifiNetworks(false);
  networking().queryWifiNetworks();

  this->initLayout();
  this->setStyleSheet(theme().qss);

  this->setMinimumSize({350, 400});
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_Escape:
    case Qt::Key_Q: {
      this->close();
    } break;
    case Qt::Key_R:
    case Qt::Key_F5: {
      networking().queryWifiNetworks();
    } break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Space: {
      return_if(this->creationTime_.addSecs(2) > QTime::currentTime());

      // select item
      auto selected = selectedItem(this->wifiList_);
      return_if(selected.isNull());
      networking().connectWifi(selected);

      // close on enter
      if (e->key() != Qt::Key_Space) {
        this->close();
      }
    } break;
  }
}

void MainWindow::initLayout() {
  this->setCentralWidget(ab::makeWidget<QWidget>([=](QWidget *w) {
    w->setLayout(ab::makeLayout<QVBoxLayout>({
        // table view containing list of wifi networks
        ab::makeWidget<QTableView>([=](QTableView *w) {
          this->wifiList_ = w;
          w->setSelectionBehavior(QAbstractItemView::SelectRows);
          w->setSelectionMode(QAbstractItemView::SingleSelection);
          w->verticalHeader()->hide();
          w->setModel(this->model_);
          w->horizontalHeader()->setSectionResizeMode(
              0, QHeaderView::ResizeToContents);
          w->horizontalHeader()->setSectionResizeMode(
              0, QHeaderView::ResizeToContents);
          w->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
          w->horizontalHeader()->hide();

          QObject::connect(
              w, &QTableView::doubleClicked, this,
              [=](const QModelIndex &index) {
                networking().connectWifi(
                    index.siblingAtColumn(ssidColumn).data().toString());
              });
        }),
    }));
  }));
}
