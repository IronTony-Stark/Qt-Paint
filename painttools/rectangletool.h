#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "linetool.h"

class RectangleTool : public LineTool {
  public:
    virtual void onMouseMove(PaintEvent*) override;
    virtual void onMouseRelease(PaintEvent*) override;

  protected:
    virtual void paint(PaintEvent* event, bool temporary) override;
};

#endif // RECTANGLETOOL_H
