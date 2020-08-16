#include <QPainter>

#include "ellipsetool.h"

void EllipseTool::onMouseMove(PaintEvent* event) {
    paint(event, true);
}

void EllipseTool::onMouseRelease(PaintEvent* event) {
    paint(event, false);

    clearToolLayer(event->toolLayer);
}

void EllipseTool::paint(PaintEvent* event, bool temporary) {
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
    painter.setBrush(event->brush);
    painter.drawEllipse(QRectF(m_originPoint, event->currentPoint));
}

