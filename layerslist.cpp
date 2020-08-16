#include "layerslist.h"
#include "ui_layerslist.h"

LayersList::LayersList(LayersModel* model, QWidget* parent) :
    QWidget(parent), ui(new Ui::LayersList), m_model(model) {
    ui->setupUi(this);

    m_view = ui->layersListView;
    m_view->setModel(m_model);

    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LayersList::on_layerSelected);
}

LayersList::~LayersList() {
    delete ui;
}

void LayersList::on_newLayerBtn_clicked() {
    emit newLayerSignal();
}

void LayersList::on_upLayerBtn_clicked() {
    emit upLayerSignal(m_view->currentIndex().row());
}

void LayersList::on_downLayerBtn_clicked() {
    emit downLayerSignal(m_view->currentIndex().row());
}

void LayersList::on_deleteLayerButton_clicked() {
    emit deleteLayerSignal(m_view->currentIndex().row());
}

void LayersList::on_layerSelected(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_UNUSED(deselected);
    emit selectedLayerSignal(selected.indexes().first().row());
}
