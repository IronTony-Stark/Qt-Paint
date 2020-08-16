#include "layersmodel.h"

LayersModel::LayersModel(QObject* parent) :
    QAbstractListModel(parent) {
}

int LayersModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return static_cast<int>(m_layers.size());
}

QVariant LayersModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    const Layer& layer = m_layers.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
    case NameRole:
        return QVariant::fromValue(layer.name());
    case ImageRole:
        return QVariant::fromValue(layer.image());
    case VisibilityRole:
        return QVariant::fromValue(layer.visible());
    default:
        return QVariant();
    }
}

bool LayersModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;

    Layer& layer = m_layers.at(index.row());

    switch (role) {
    case Qt::EditRole:
    case NameRole:
        layer.setName(value.toString());
        emit dataChanged(index, index, { NameRole });
        break;
    case ImageRole:
        layer.setImage(value.value<QImage>());
        emit dataChanged(index, index, { ImageRole });
        break;
    case VisibilityRole:
        layer.setVisible(value.toBool());
        emit dataChanged(index, index, { VisibilityRole });
        break;
    }

    return true;
}

Qt::ItemFlags LayersModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool LayersModel::insertRows(int row, int count, const QModelIndex& parent) {
    Q_UNUSED(parent)

    if (row < 0 || row > static_cast<int>(m_layers.size()))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++)
        m_layers.insert(m_layers.begin() + row, Layer());

    endInsertRows();

    return true;
}

bool LayersModel::removeRows(int row, int count, const QModelIndex& parent) {
    Q_UNUSED(parent)

    if (row < 0 || row >= static_cast<int>(m_layers.size()))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++)
        m_layers.erase(m_layers.begin() + row);

    endRemoveRows();

    return true;
}

Layer& LayersModel::get(int index) {
    return m_layers[index];
}

void LayersModel::moveUp(const int index) {
    if (index > 0 && index < static_cast<int>(m_layers.size())) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);

        std::swap(m_layers[index], m_layers[index - 1]);

        endMoveRows();
    }
}

void LayersModel::moveDown(const int index) {
    if (index >= 0 && index < static_cast<int>(m_layers.size() - 1))
        moveUp(index + 1);
}
