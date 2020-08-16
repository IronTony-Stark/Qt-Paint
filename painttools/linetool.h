#ifndef LINETOOL_H
#define LINETOOL_H

#include "painttool.h"

class LineTool : public PaintTool {

  public:
    virtual void onMousePress(PaintEvent*) override;
    virtual void onMouseRelease(PaintEvent*) override;
    virtual void onMouseMove(PaintEvent*) override;

  protected:
    QPoint m_originPoint;

    virtual void paint(PaintEvent* event, bool temporary);
    void clearToolLayer(QPixmap*);
};

#endif // LINETOOL_H
