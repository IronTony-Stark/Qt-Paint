#ifndef PAINTTOOL_H
#define PAINTTOOL_H

#include "paintevent.h"

class PaintTool {
  public:
    virtual void onMousePress(PaintEvent*) = 0;
    virtual void onMouseMove(PaintEvent*) = 0;
    virtual void onMouseRelease(PaintEvent*) = 0;
};

#endif // PAINTTOOL_H
