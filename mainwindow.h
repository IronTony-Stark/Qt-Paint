#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QCheckBox>
#include <QDockWidget>
#include <QSpinBox>
#include <QSlider>

#include "paintarea.h"
#include "colorlabel.h"
#include "layerslist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    PaintArea* paintArea() const;

  private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionCopy_triggered();
    void on_actionPaste_triggered();

    void on_actionPen_triggered();
    void on_actionEraser_triggered();
    void on_actionLine_triggered();
    void on_actionRectangle_triggered();
    void on_actionEllipse_triggered();

    void on_actionLayers_triggered(bool checked);

  private:
    Ui::MainWindow* ui;
    LayersModel* m_layersModel;
    PaintArea* m_paintArea;

    QSpinBox* m_strokeSizeSpinBox;
    ColorLabel m_strokeColorLabel;
    ColorLabel m_fillColorLabel;
    QSlider* m_zoomSlider;
    QCheckBox* m_antialiasingCheckBox;

    QString m_fileName;
    QString m_filePath;

    QDockWidget m_dock;
    LayersList* m_layersList;

    void closeEvent(QCloseEvent*) override;

    void setupToolbar();
    void setupConnections();
    void setupDock();
    void setupDefaultValues();

    bool saveExistingFile();
    bool saveNewFile();
    int confirmSaveAction();

};

#endif // MAINWINDOW_H
