#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "linetool.h"

class EllipseTool : public LineTool {

  public:
    virtual void onMouseMove(PaintEvent*) override;
    virtual void onMouseRelease(PaintEvent*) override;

  protected:
    virtual void paint(PaintEvent* event, bool temporary) override;
};

#endif // ELLIPSETOOL_H
