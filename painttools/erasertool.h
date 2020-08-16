#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include <QPoint>
#include <QPen>
#include "painttool.h"

class EraserTool : public PaintTool {

  public:
    EraserTool();

    virtual void onMousePress(PaintEvent*) override;
    virtual void onMouseRelease(PaintEvent*) override;
    virtual void onMouseMove(PaintEvent*) override;

  private:
    QPen m_eraser;

    void eraseLine(PaintEvent*);
    void erasePoint(PaintEvent*);
};

#endif // ERASERTOOL_H
