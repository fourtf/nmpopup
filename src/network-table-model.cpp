#include "network-table-model.hpp"

#include <QColor>

NetworkTableModel::NetworkTableModel() {}

void NetworkTableModel::setItems(const std::vector<WifiNetwork> &items) {
  beginRemoveRows({}, 0, int(this->items_.size()));
  this->items_.clear();
  endRemoveRows();

  beginInsertRows({}, 0, int(items.size()));
  this->items_ = items;
  endInsertRows();
}

int NetworkTableModel::rowCount(const QModelIndex &) const {
  return int(this->items_.size());
}

int NetworkTableModel::columnCount(const QModelIndex &) const { return 3; }

QVariant NetworkTableModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() > int(this->items_.size())) {
    return {};
  }

  if (role == Qt::DisplayRole) {
    auto &&row = this->items_[size_t(index.row())];

    switch (index.column()) {
      case 0:
        return row.bars;
      case 1:
        return row.ssid;
      case 2:
        return row.security;
      default:
        return {};
    }
  } else if (role == Qt::ForegroundRole) {
    auto &&row = this->items_[size_t(index.row())];

    switch (row.bars.count('_')) {
      case 0:
        return QColor("#3e2");
      case 1:
        return QColor("#9e2");
      case 2:
        return QColor("#c92");
      case 3:
        return QColor("#e34");
      default:
        return {};
    }
  } else {
    return {};
  }
}
