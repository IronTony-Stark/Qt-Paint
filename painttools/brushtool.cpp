#include <QPainter>

#include "brushtool.h"

void BrushTool::onMousePress(PaintEvent* event) {
    paintPoint(event);
}

void BrushTool::onMouseMove(PaintEvent* event) {
    paintLine(event);
}

void BrushTool::onMouseRelease(PaintEvent*) {

}

void BrushTool::paintLine(PaintEvent* event) {
    QPainter painter(event->selectedLayer);
    if (event->antialiasingEnabled)
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(event->pen);
    painter.drawLine(event->previousPoint, event->currentPoint);
}

void BrushTool::paintPoint(PaintEvent* event) {
    QPainter painter(event->selectedLayer);
    if (event->antialiasingEnabled)
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(event->pen);
    painter.drawPoint(event->currentPoint);
}
