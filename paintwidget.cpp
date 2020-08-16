#include <QPainter>
#include <QMouseEvent>

#include "paintwidget.h"
#include "paintarea.h"

PaintWidget::PaintWidget(LayersModel* layers, QWidget* parent) :
    QWidget(parent), m_layers(layers), m_selectedLayer(0) {
}

void PaintWidget::mousePressEvent(QMouseEvent* event) {
    Layer& layer = m_layers->get(m_selectedLayer);

    if (event->button() == Qt::LeftButton && layer.visible())
        emit mousePress(event->pos(), layer.image());
}

void PaintWidget::mouseMoveEvent(QMouseEvent* event) {
    Layer& layer = m_layers->get(m_selectedLayer);
    emit mouseMove(event->pos(), layer.image());
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        Layer& layer = m_layers->get(m_selectedLayer);
        emit mouseRelease(event->pos(), layer.image());
    }
}

void PaintWidget::paintEvent(QPaintEvent* event) {
    auto eventRect = event->rect();
    QImage* image = m_layers->data(m_layers->index(0), LayersModel::ImageRole).value<QImage*>();

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    qreal scaleX = size().width() / qreal(image->size().width());
    qreal scaleY = size().height() / qreal(image->size().height());
    qreal scale = qMin(scaleX, scaleY);
    auto imageRect = QRectF(eventRect.topLeft() / scale, eventRect.bottomRight() / scale);

    for (int i = 0; i < m_layers->rowCount(); i++) {
        Layer& layer = m_layers->get(i);

        if (layer.visible())
            painter.drawImage(eventRect, *layer.image(), imageRect);
    }
    painter.drawPixmap(eventRect, m_toolLayer, imageRect);
}

void PaintWidget::setSingleLayer(const QImage& image) {
    m_layers->removeRows(0, m_layers->rowCount());
    m_layers->insertRows(0, 1);
    Layer& layer = m_layers->get(0);
    layer.setImage(image);
    m_selectedLayer = 0;
    resize(image.size());
    setToolLayer(image.size());
    update();
}

void PaintWidget::addLayer(const QImage& image, const QString& name) {
    int index = m_layers->rowCount() - 1;
    m_layers->insertRows(index, 1);

    auto currentSize = m_layers->data(m_layers->index(0), LayersModel::ImageRole).value<QImage*>()->size();

    QPixmap pixmap(currentSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.drawImage(QPoint(0,0), image);
    QImage convertedImage = pixmap.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);

    Layer& layer = m_layers->get(index);
    layer.setName(name);
    layer.setImage(convertedImage);

    m_selectedLayer = m_layers->rowCount() - 1;
}

QPixmap* PaintWidget::toolLayer() {
    return &m_toolLayer;
}

void PaintWidget::setToolLayer(const QSize& size) {
    m_toolLayer = QPixmap(size);
    m_toolLayer.fill(Qt::transparent);
}

QImage PaintWidget::renderImage() {
    auto pixmap = QPixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    for (int i = 0; i < m_layers->rowCount(); i++) {
        Layer& layer = m_layers->get(i);

        if (layer.visible())
            painter.drawImage(QPoint(0, 0), *layer.image());
    }

    return pixmap.toImage();
}

void PaintWidget::rezoom(int zoom) {
    if (m_layers->rowCount() == 0)
        return;

    Layer& layer = m_layers->get(0);
    auto layerSize = layer.image()->size();
    auto newSize = layerSize * (zoom / 100.0);
    resize(newSize);
}

void PaintWidget::on_layerSelected(int layer) {
    m_selectedLayer = layer;
}

void PaintWidget::on_newLayerCreated() {
    addLayer(QImage(), "New Layer");
}
