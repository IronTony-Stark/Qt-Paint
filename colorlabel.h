#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QColorDialog>

class ColorLabel : public QLabel {
    Q_OBJECT

  public:
    ColorLabel(QWidget* parent = nullptr, const QColor& color = Qt::white);
    ~ColorLabel();
    void openColorSelectionDialog();
    QColor color() const;
    QColorDialog* dialog() const;

  public slots:
    void setColor(const QColor& color);

  private:
    QColorDialog* m_dialog;
    QColor m_color;
    void mouseReleaseEvent(QMouseEvent* ev) override;
};

#endif // COLORLABEL_H
