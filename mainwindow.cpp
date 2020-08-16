#include <QColorDialog>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDockWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newimagedialog.h"
#include "paintarea.h"

static const QString DEFAULT_FILE_NAME = "Untitled";

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_fileName = DEFAULT_FILE_NAME;
    m_layersModel = new LayersModel();
    m_paintArea = new PaintArea(m_layersModel);
    m_layersList = new LayersList(m_layersModel);

    setCentralWidget(m_paintArea);

    setupToolbar();
    setupConnections();
    setupDock();
    setupDefaultValues();

    m_paintArea->setBlankLayer();
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_strokeSizeSpinBox;
    delete m_zoomSlider;
    delete m_antialiasingCheckBox;
    delete m_layersList;
}

PaintArea* MainWindow::paintArea() const {
    return m_paintArea;
}

void MainWindow::setupConnections() {
    connect(m_strokeSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_paintArea, &PaintArea::setPenWidth);
    connect(m_strokeColorLabel.dialog(), &QColorDialog::colorSelected, m_paintArea, &PaintArea::setPenColor);
    connect(m_fillColorLabel.dialog(), &QColorDialog::colorSelected, m_paintArea, &PaintArea::setBrushColor);
    connect(m_zoomSlider, &QSlider::valueChanged, m_paintArea, &PaintArea::setZoom);
    connect(m_antialiasingCheckBox, &QCheckBox::toggled, m_paintArea, &PaintArea::setAntialiasingEnabled);
    connect(m_layersList, &LayersList::selectedLayerSignal, m_paintArea->getPaintWidget(), &PaintWidget::on_layerSelected);
    connect(m_layersList, &LayersList::newLayerSignal, m_paintArea->getPaintWidget(), &PaintWidget::on_newLayerCreated);
}

void MainWindow::setupToolbar() {
    ui->toolbar->addAction(ui->actionNew);
    ui->toolbar->addAction(ui->actionOpen);
    ui->toolbar->addAction(ui->actionSave);
    ui->toolbar->addSeparator();

    ui->toolbar->addAction(ui->actionCopy);
    ui->toolbar->addAction(ui->actionPaste);
    ui->toolbar->addSeparator();

    ui->toolbar->addAction(ui->actionPen);
    ui->toolbar->addAction(ui->actionEraser);
    ui->toolbar->addAction(ui->actionLine);
    ui->toolbar->addAction(ui->actionRectangle);
    ui->toolbar->addAction(ui->actionEllipse);
    ui->toolbar->addSeparator();

    ui->toolbar->addWidget(new QLabel("Stroke Size"));
    m_strokeSizeSpinBox = new QSpinBox(this);
    m_strokeSizeSpinBox->setRange(PaintArea::MIN_PEN_WIDTH, PaintArea::MAX_PEN_WIDTH);
    ui->toolbar->addWidget(m_strokeSizeSpinBox);
    ui->toolbar->addSeparator();

    ui->toolbar->addWidget(new QLabel("Stroke color"));
    ui->toolbar->addWidget(&m_strokeColorLabel);
    ui->toolbar->addWidget(new QLabel("Fill color"));
    ui->toolbar->addWidget(&m_fillColorLabel);
    ui->toolbar->addSeparator();

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setRange(PaintArea::MIN_ZOOM, PaintArea::MAX_ZOOM);
    m_zoomSlider->setOrientation(Qt::Horizontal);
    ui->toolbar->addWidget(m_zoomSlider);
    ui->toolbar->addSeparator();

    m_antialiasingCheckBox = new QCheckBox("Antialiasing", this);
    m_antialiasingCheckBox->setChecked(m_paintArea->antialiasingEnabled());
    ui->toolbar->addWidget(m_antialiasingCheckBox);
    ui->toolbar->addSeparator();

    ui->toolbar->addAction(ui->actionLayers);
}

void MainWindow::setupDock() {
    static QLabel* title = new QLabel("Layers");
    title->setAlignment(Qt::AlignHCenter);
    m_dock.setTitleBarWidget(title);
    m_dock.setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_dock.setWidget(m_layersList);
    addDockWidget(Qt::RightDockWidgetArea, &m_dock);
}

void MainWindow::setupDefaultValues() {
    m_strokeSizeSpinBox->setValue(PaintArea::DEFAULT_PEN_WIDTH);
    m_strokeColorLabel.setColor(m_paintArea->pen().color());
    m_fillColorLabel.setColor(m_paintArea->brush().color());
    m_zoomSlider->setValue(PaintArea::DEFAULT_ZOOM);
}

void MainWindow::on_actionNew_triggered() {
    switch (confirmSaveAction()) {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        break;
    case QMessageBox::Cancel:
        return;
    }

    NewImageDialog* dialog = new NewImageDialog(this);
    dialog->setWindowModality(Qt::WindowModal);
    if (dialog->exec()) {
        m_paintArea->setBlankLayer(dialog->width(), dialog->height(), dialog->color());
        m_fileName = DEFAULT_FILE_NAME;
        m_filePath = DEFAULT_FILE_NAME;
    }

    delete dialog;
}

void MainWindow::on_actionOpen_triggered() {
    auto file = QFileDialog::getOpenFileName(this, "Open image");
    if (file.isNull())
        return;

    if (m_paintArea->loadImage(file)) {
        ui->statusBar->showMessage("Opened " + file);
        m_filePath = file;
        m_fileName = QFileInfo(file).fileName();
    } else {
        ui->statusBar->showMessage("Can't open " + file);
    }
}

void MainWindow::on_actionSave_triggered() {
    static const QString SAVE_SUCCESS = "Saved image to ";
    static const QString SAVE_FAILED = "Can't save to ";
    bool result = QFileInfo(m_filePath).exists() ? saveExistingFile() : saveNewFile();
    ui->statusBar->showMessage((result ? SAVE_SUCCESS : SAVE_FAILED) + m_fileName);
}

void MainWindow::on_actionCopy_triggered() {
    m_paintArea->copyImage();
}

void MainWindow::on_actionPaste_triggered() {
    switch (confirmSaveAction()) {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        break;
    case QMessageBox::Cancel:
        return;
    }

    if (!m_paintArea->pasteImage())
        ui->statusBar->showMessage("Can't paste image from clipboard");
}

void MainWindow::on_actionPen_triggered() {
    m_paintArea->setSelectedTool(0);
}

void MainWindow::on_actionEraser_triggered() {
    m_paintArea->setSelectedTool(1);
}

void MainWindow::on_actionLine_triggered() {
    m_paintArea->setSelectedTool(2);
}

void MainWindow::on_actionRectangle_triggered() {
    m_paintArea->setSelectedTool(3);
}

void MainWindow::on_actionEllipse_triggered() {
    m_paintArea->setSelectedTool(4);
}

void MainWindow::on_actionLayers_triggered(bool checked) {
    m_dock.setVisible(checked);
}

bool MainWindow::saveExistingFile() {
    return (m_paintArea->getRenderImage().save(m_filePath));
}

bool MainWindow::saveNewFile() {
    m_filePath = QFileDialog::getSaveFileName(this, "Save image");
    if (m_filePath.isNull())
        return false;

    m_fileName = QFileInfo(m_filePath).fileName();

    return saveExistingFile();
}

int MainWindow::confirmSaveAction() {
    return QMessageBox::question(this, "", "Save changes to " + m_fileName + "?",
                                 QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
}

void MainWindow::closeEvent(QCloseEvent* e) {
    switch (confirmSaveAction()) {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        break;
    case QMessageBox::Cancel:
        e->setAccepted(false);
        break;
    }
}
