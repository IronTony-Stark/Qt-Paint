#include <QPainter>

#include "linetool.h"
#include "paintarea.h"

void LineTool::onMousePress(PaintEvent* event) {
    m_originPoint = event->currentPoint;
}

void LineTool::onMouseMove(PaintEvent* event) {
    paint(event, true);
}

void LineTool::onMouseRelease(PaintEvent* event) {
    paint(event, false);

    clearToolLayer(event->toolLayer);
}

void LineTool::paint(PaintEvent* event, bool temporary) {
    clearToolLayer(event->toolLayer);

    QPaintDevice* device;
    if (temporary)
        device = event->toolLayer;
    else
        device = event->selectedLayer;

    QPainter painter(device);
    if (event->antialiasingEnabled)
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(event->pen);
    painter.drawLine(m_originPoint, event->currentPoint);
}

void LineTool::clearToolLayer(QPixmap* layer) {
    layer->fill(Qt::transparent);
}

