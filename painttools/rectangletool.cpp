#include <QPainter>

#include "rectangletool.h"

void RectangleTool::onMouseMove(PaintEvent* event) {
    paint(event, true);
}

void RectangleTool::onMouseRelease(PaintEvent* event) {
    paint(event, false);

    clearToolLayer(event->toolLayer);
}

void RectangleTool::paint(PaintEvent* event, bool temporary) {
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
    painter.drawRect(QRect(m_originPoint, event->currentPoint));
}
