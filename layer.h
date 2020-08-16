#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QString>

static const QString LAYER_NAME = QStringLiteral("Layer");

class Layer {
  public:
    Layer(const QImage& image = QImage(), const QString& name = LAYER_NAME, bool m_visible = true);

    QString name() const;
    void setName(const QString& name);

    bool visible() const;
    void setVisible(bool value);

    const QImage* image() const;
    QImage* image();
    void setImage(const QImage& image);

  private:
    QImage m_image;
    QString m_name;
    bool m_visible;
};

#endif // LAYER_H
