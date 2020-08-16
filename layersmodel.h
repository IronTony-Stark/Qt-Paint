#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include <QAbstractListModel>

#include "layer.h"

class LayersModel : public QAbstractListModel {
    Q_OBJECT

  public:
    explicit LayersModel(QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    Layer& get(int);

    void moveUp(const int index);
    void moveDown(const int index);

    enum LayerRoles {
        NameRole = Qt::UserRole + 1,
        ImageRole,
        VisibilityRole,
    };

  private:
    std::vector<Layer> m_layers;
};

#endif // LAYERSMODEL_H
