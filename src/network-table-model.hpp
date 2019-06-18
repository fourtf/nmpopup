#pragma once

#include <QAbstractTableModel>

#include "networking.hpp"

class NetworkTableModel : public QAbstractTableModel {
public:
  NetworkTableModel();

  void setItems(const std::vector<WifiNetwork> &);

  // QAbstractItemModel interface
  int rowCount(const QModelIndex &parent = {}) const override;
  int columnCount(const QModelIndex &parent = {}) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

private:
  std::vector<WifiNetwork> items_;
};
