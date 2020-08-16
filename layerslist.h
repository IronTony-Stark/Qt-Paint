#ifndef LAYERSLIST_H
#define LAYERSLIST_H

#include <QWidget>
#include <QListView>
#include <QStringListModel>

#include "layersmodel.h"

namespace Ui {
class LayersList;
}

class LayersList : public QWidget {
    Q_OBJECT

  public:
    explicit LayersList(LayersModel* model, QWidget* parent = 0);
    ~LayersList();

  signals:
    void selectedLayerSignal(int);
    void newLayerSignal();
    void upLayerSignal(int);
    void downLayerSignal(int);
    void deleteLayerSignal(int);

  public slots:
    void on_newLayerBtn_clicked();
    void on_upLayerBtn_clicked();
    void on_downLayerBtn_clicked();
    void on_deleteLayerButton_clicked();
    void on_layerSelected(const QItemSelection& selected, const QItemSelection& deselected);

  private:
    Ui::LayersList* ui;
    QListView* m_view;
    LayersModel* m_model;

    void listItemSwap(bool up);
};

#endif // LAYERSLIST_H
