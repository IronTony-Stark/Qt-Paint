#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QOpenGLWidget>
#include <QScrollArea>
#include <QPen>
#include <QVector>
#include <QPixmap>
#include <QLabel>

#include "paintwidget.h"
#include "painttools/brushtool.h"
#include "painttools/linetool.h"
#include "painttools/rectangletool.h"
#include "painttools/erasertool.h"
#include "painttools/ellipsetool.h"
#include "layer.h"

class PaintArea : public QScrollArea {
    Q_OBJECT

  public:
    enum PaintLimits {
        DEFAULT_SELECTED_TOOL = 0,

        MIN_PEN_WIDTH = 0,
        MAX_PEN_WIDTH = 100,
        DEFAULT_PEN_WIDTH = 7,
        DEFAULT_PEN_COLOR = Qt::black,

        MIN_ZOOM = 1,
        MAX_ZOOM = 400,
        DEFAULT_ZOOM = 100,

        DEFAULT_ANTIALIASING = false,

        MIN_IMAGE_SIZE = 1,
        MAX_IMAGE_SIZE = 2000,
        DEFAULT_WIDTH = 800,
        DEFAULT_HEIGHT = 600
    };

    explicit PaintArea(LayersModel* layers, QWidget* parent = 0);

    ~PaintArea();

    void setBlankLayer(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT, QColor fill = QColor(Qt::white));
    bool loadImage(const QString&);

    bool pasteImage();
    void copyImage();

    void setSelectedTool(int i);
    int selectedTool() const;
    const QVector<PaintTool*>* tools() const;

    bool antialiasingEnabled() const;
    void setAntialiasingEnabled(bool antialiasingEnabled);

    QPen pen() const;
    QBrush brush() const;
    void setBrushColor(const QColor&);

    int zoom() const;
    void setZoom(int zoom);

    QImage getRenderImage();

    QSize imageSize() const;

    PaintWidget* getPaintWidget() const;

  public slots:
    void mousePress(const QPoint&, QImage*);
    void mouseMove(const QPoint&, QImage*);
    void mouseRelease(const QPoint&, QImage*);

    void setPenColor(const QColor& color);
    void setPenWidth(int);

  private:
    PaintWidget* m_paintWidget;
    int m_selectedTool;
    QPen m_pen;
    QBrush m_brush;
    QPoint m_prevPoint;
    int m_zoom;
    bool m_antialiasingEnabled;
    bool m_isPainting;

    const QVector<PaintTool*> m_tools {
        new BrushTool(),
        new EraserTool(),
        new LineTool(),
        new RectangleTool(),
        new EllipseTool(),
    };

    PaintEvent* createPaintEvent(const QPoint&, QImage*);
    QPoint adjustedPos(const QPoint&);
};

#endif // PAINTAREA_H
