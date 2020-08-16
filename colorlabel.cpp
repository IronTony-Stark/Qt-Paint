#include <QPalette>
#include <QMouseEvent>

#include "colorlabel.h"

ColorLabel::ColorLabel(QWidget* parent, const QColor& color) :
    QLabel(parent), m_dialog(new QColorDialog) {
    connect(m_dialog, &QColorDialog::colorSelected, this, &ColorLabel::setColor);
    setColor(color);
    setText("         "); // defining the width of the label
}

ColorLabel::~ColorLabel() {
    delete m_dialog;
}

void ColorLabel::setColor(const QColor& color) {
    m_color = color;
    setStyleSheet(QString("border-style:inset;border-width:1px;border-color:#7A7A7A;")
                  + "background:" + m_color.name());
    setWindowOpacity(m_color.alpha() / 255);
}

void ColorLabel::openColorSelectionDialog() {
    m_dialog->setOption(QColorDialog::ShowAlphaChannel);
    m_dialog->setCurrentColor(m_color);
    m_dialog->show();
}

QColor ColorLabel::color() const {
    return m_color;
}

QColorDialog* ColorLabel::dialog() const {
    return m_dialog;
}

void ColorLabel::mouseReleaseEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::LeftButton)
        openColorSelectionDialog();
}
