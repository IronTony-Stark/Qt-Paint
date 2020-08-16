#ifndef PAINTEVENT_H
#define PAINTEVENT_H

#include <QPixmap>
#include <QBrush>
#include <QPen>

struct PaintEvent {
    QImage* selectedLayer;
    QPixmap* toolLayer;
    QPen pen;
    QBrush brush;
    QPoint currentPoint;
    QPoint previousPoint;
    bool antialiasingEnabled;
};

#endif // PAINTEVENT_H
