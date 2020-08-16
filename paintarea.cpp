#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QClipboard>

#include "paintarea.h"
#include "paintevent.h"

static const QColor DEFAULT_BRUSH = QColor(0xFF, 0xFF, 0xFF, 0xFF);

PaintArea::PaintArea(LayersModel* layers, QWidget* parent) :
    QScrollArea(parent), m_paintWidget(new PaintWidget(layers)) {
    setAlignment(Qt::AlignCenter);
    setBackgroundRole(QPalette::Midlight);

    m_selectedTool = DEFAULT_SELECTED_TOOL;
    m_pen.setWidth(DEFAULT_PEN_WIDTH);
    m_pen.setColor(DEFAULT_PEN_COLOR);
    m_brush = DEFAULT_BRUSH;
    m_zoom = DEFAULT_ZOOM;
    m_antialiasingEnabled = DEFAULT_ANTIALIASING;

    setWidget(m_paintWidget);

    connect(m_paintWidget, &PaintWidget::mouseMove, this, &PaintArea::mouseMove);
    connect(m_paintWidget, &PaintWidget::mouseRelease, this, &PaintArea::mouseRelease);
    connect(m_paintWidget, &PaintWidget::mousePress, this, &PaintArea::mousePress);
}

PaintArea::~PaintArea() {
    delete m_paintWidget;
}

void PaintArea::setBlankLayer(int width, int height, QColor fill) {
    QPixmap pixmap(width, height);
    pixmap.fill(fill);

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    m_paintWidget->setSingleLayer(image);
}

bool PaintArea::loadImage(const QString& path) {
    auto image = QImage();
    bool success = image.load(path);
    if (!success)
        return false;

    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    m_paintWidget->setSingleLayer(image);
    return true;
}

void PaintArea::mousePress(const QPoint& pos, QImage* layer) {
    m_isPainting = true;

    auto adjusted = adjustedPos(pos);
    m_prevPoint = adjusted;
    PaintEvent* event(createPaintEvent(adjusted, layer));
    m_tools[m_selectedTool]->onMousePress(event);

    delete event;
    m_paintWidget->update();
}

void PaintArea::mouseMove(const QPoint& pos, QImage* layer) {
    if (m_isPainting) {
        auto adjusted = adjustedPos(pos);

        PaintEvent* event(createPaintEvent(adjusted, layer));
        m_tools[m_selectedTool]->onMouseMove(event);
        m_prevPoint = adjusted;

        delete event;
        m_paintWidget->update();
    }
}

void PaintArea::mouseRelease(const QPoint& pos, QImage* layer) {
    m_isPainting = false;

    PaintEvent* event(createPaintEvent(adjustedPos(pos), layer));
    m_tools[m_selectedTool]->onMouseRelease(event);

    delete event;
    m_paintWidget->update();
}

void PaintArea::copyImage() {
    auto clipboard = QApplication::clipboard();
    clipboard->setImage(getRenderImage());
}

bool PaintArea::pasteImage() {
    auto clipboard = QApplication::clipboard();
    QImage image = clipboard->image();
    if (image.isNull())
        return false;

    m_paintWidget->addLayer(image, "Pasted");

    return true;
}

QPen PaintArea::pen() const {
    return m_pen;
}

void PaintArea::setPenColor(const QColor& color) {
    m_pen.setColor(color);
}

void PaintArea::setPenWidth(int value) {
    m_pen.setWidth(value);
}

QBrush PaintArea::brush() const {
    return m_brush;
}

void PaintArea::setBrushColor(const QColor& color) {
    m_brush.setColor(color.toRgb());
}

int PaintArea::zoom() const {
    return m_zoom;
}

void PaintArea::setZoom(int zoom) {
    m_zoom = zoom;
    m_paintWidget->rezoom(zoom);
}

const QVector<PaintTool*>* PaintArea::tools() const {
    return &m_tools;
}

int PaintArea::selectedTool() const {
    return m_selectedTool;
}

void PaintArea::setSelectedTool(int i) {
    m_selectedTool = i;
}

bool PaintArea::antialiasingEnabled() const {
    return m_antialiasingEnabled;
}

void PaintArea::setAntialiasingEnabled(bool antialiasingEnabled) {
    m_antialiasingEnabled = antialiasingEnabled;
}

QImage PaintArea::getRenderImage() {
    return m_paintWidget->renderImage();
}

QSize PaintArea::imageSize() const {
    return m_paintWidget->size();
}

PaintWidget* PaintArea::getPaintWidget() const {
    return m_paintWidget;
}

PaintEvent* PaintArea::createPaintEvent(const QPoint& point, QImage* layer) {
    PaintEvent* event = new PaintEvent();

    event->selectedLayer = layer;
    event->toolLayer = m_paintWidget->toolLayer();
    event->pen = m_pen;
    event->brush = m_brush;
    event->currentPoint = point;
    event->previousPoint = m_prevPoint;
    event->antialiasingEnabled = m_antialiasingEnabled;

    return event;
}

QPoint PaintArea::adjustedPos(const QPoint& pos) {
    double factor = 100.0 / m_zoom;
    return pos * factor;
}
