#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include <QPoint>
#include "painttool.h"

class BrushTool : public PaintTool {

  public:
    virtual void onMousePress(PaintEvent*) override;
    virtual void onMouseRelease(PaintEvent*) override;
    virtual void onMouseMove(PaintEvent*) override;

  private:
    void paintLine(PaintEvent* event);
    void paintPoint(PaintEvent* event);
};

#endif // BRUSHTOOL_H
