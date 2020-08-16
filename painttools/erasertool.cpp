#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QBitmap>

#include "erasertool.h"
#include "paintarea.h"

EraserTool::EraserTool() {
    m_eraser.setColor(Qt::white);
}

void EraserTool::onMousePress(PaintEvent* event) {
    erasePoint(event);
}

void EraserTool::onMouseMove(PaintEvent* event) {
    eraseLine(event);
}

void EraserTool::onMouseRelease(PaintEvent*) {

}

void EraserTool::erasePoint(PaintEvent* event) {
    QPainter painter(event->selectedLayer);
    if (event->antialiasingEnabled)
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    m_eraser.setWidth(event->pen.width());
    painter.setPen(m_eraser);
    painter.drawPoint(event->currentPoint);
}

void EraserTool::eraseLine(PaintEvent* event) {
    QPainter painter(event->selectedLayer);
    if (event->antialiasingEnabled)
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    m_eraser.setWidth(event->pen.width());
    painter.setPen(m_eraser);
    painter.drawLine(event->previousPoint, event->currentPoint);
}
