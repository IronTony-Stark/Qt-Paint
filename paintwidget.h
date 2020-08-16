#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>

#include "layer.h"
#include "layersmodel.h"

class PaintWidget : public QWidget {
    Q_OBJECT

  public:
    explicit PaintWidget(LayersModel* layers, QWidget* parent = 0);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    void setSingleLayer(const QImage&);
    void addLayer(const QImage&, const QString&);

    QPixmap* toolLayer();
    void setToolLayer(const QSize&);

    QImage renderImage();

    void rezoom(int zoom);


  signals:
    void mousePress(const QPoint&, QImage*);
    void mouseMove(const QPoint&, QImage*);
    void mouseRelease(const QPoint&, QImage*);

  public slots:
    void on_layerSelected(int);
    void on_newLayerCreated();

  private:
    LayersModel* m_layers;
    int m_selectedLayer;
    QPixmap m_toolLayer;
};

#endif // PAINTWIDGET_H
