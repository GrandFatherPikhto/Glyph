#include <QSettings>
#include <QSharedPointer>
#include <QToolButton>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "glyphwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_glyphToolBar(nullptr)
    , m_glyphManager(nullptr)
    , m_dockGlyph(nullptr)
    , m_dockGlyphSelector(nullptr)
    , m_fontLabel(nullptr)
    , m_glyphSizeLabel(nullptr)
    , m_gridSizeLabel(nullptr)
    , m_glyphRectLabel(nullptr)
    , m_charLabel(nullptr)
    , m_mainLayout(nullptr)
    , m_glyphWidget(nullptr)
    , m_gridEnable(nullptr)
    , m_templateLayerEnable(nullptr)
    , m_userLayerEnable(nullptr)
    , m_previewLayerEnable(nullptr)
    , m_glyphRectLayerEnable(nullptr)
    , m_baselineLayerEnable(nullptr)
    , m_bitmapRectLayerEnable(nullptr)
    , m_addBottomCells(nullptr)
    , m_addLeftCells(nullptr)
    , m_pasteGlyphToUserLayer(nullptr)
    , m_clearUserLayer(nullptr)
{
    ui->setupUi(this);
    m_glyphManager = new GlyphManager(this);

    setupGlyphWidget();
    setupDockPanels();
    setupGlyphToolBar();
    setupStatusBar();
    setupSignals ();
}

MainWindow::~MainWindow()
{
    delete m_glyphManager;
    delete ui;
}

void MainWindow::setupSignals ()
{
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::slotActionQuitTriggered);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphChanged, m_glyphWidget, &GlyphWidget::setGlyphMeta);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphChanged, this, [=](QSharedPointer<GlyphMeta> glyphMeta){
        setStatusBarFontName(glyphMeta->font());
        setStatusBarCharacter(glyphMeta->character());
        setStatusBarGlyphSize(glyphMeta->glyphSize());
        setStatusBarBitmapDimension(glyphMeta->bitmapDimension());
    });
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, this, &MainWindow::setStatusBarFontName);
    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, this, &MainWindow::setStatusBarCharacter);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphSizeChanged, this, &MainWindow::setStatusBarGlyphSize);
    QObject::connect(m_dockGlyph, &DockGlyph::bitmapDimensionChanged, this, &MainWindow::setStatusBarBitmapDimension);
    QObject::connect(this, &MainWindow::templateLayerEnable, m_glyphWidget, &GlyphWidget::enableTemplateLayer);
    QObject::connect(this, &MainWindow::previewLayerEnable, m_glyphWidget, &GlyphWidget::enablePreviewLayer);
    QObject::connect(this, &MainWindow::gridEnable, m_glyphWidget, &GlyphWidget::enableGrid);
    QObject::connect(this, &MainWindow::userLayerEnable, m_glyphWidget, &GlyphWidget::enableUserLayer);
    QObject::connect(this, &MainWindow::glyphRectLayerEnable, m_glyphWidget, &GlyphWidget::enableGlyphRectLayer);
    QObject::connect(this, &MainWindow::baselineLayerEnable, m_glyphWidget, &GlyphWidget::enableBaselineLayer);
    QObject::connect(this, &MainWindow::bitmapRectLayerEnable, m_glyphWidget, &GlyphWidget::enableBitmapRectLayer);
    QObject::connect(this, &MainWindow::leftGridCells, m_glyphWidget, &GlyphWidget::setLeftGridCells);
    QObject::connect(this, &MainWindow::bottomGridCells, m_glyphWidget, &GlyphWidget::setBottomGridCells);
    QObject::connect(this, &MainWindow::pasteGlyphToUserLayer, m_glyphWidget, &GlyphWidget::pasteGlyphToUserLayer);
    QObject::connect(this, &MainWindow::clearUserLayer, m_glyphWidget, &GlyphWidget::clearUserLayer);
    QObject::connect(m_dockGlyphSelector, &DockGlyphSelector::glyphChanged, m_glyphWidget, &GlyphWidget::setGlyphMeta);
}

void MainWindow::setupGlyphWidget ()
{
    m_mainLayout = new QGridLayout(this);
    m_glyphWidget = new GlyphWidget(m_glyphManager, this);
    m_mainLayout->addWidget(m_glyphWidget);
    ui->centralwidget->setLayout(m_mainLayout);
}

void MainWindow::setupDockPanels ()
{
    m_dockGlyph = new DockGlyph(m_glyphManager, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyph);
    m_dockGlyphSelector = new DockGlyphSelector(m_glyphManager, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyphSelector);
    // tabifiedDockWidgetActivated(m_dockGlyph, m_dockGlyphSelector);
}

void MainWindow::setupGlyphToolBar()
{
    m_glyphToolBar = new QToolBar("Glyph Toolbar", this);
    m_glyphToolBar->setObjectName("GlyphToolbar");
    m_templateLayerEnable = new QAction(QIcon(":/button/icons/template"), "Template Layer", this);
    m_templateLayerEnable->setCheckable(true);
    QObject::connect(m_templateLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit templateLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_templateLayerEnable);

    m_gridEnable = new QAction(QIcon(":/button/icons/grid"), "Grid Enable", this);
    m_gridEnable->setCheckable(true);
    QObject::connect(m_gridEnable, &QAction::toggled, this, [=](bool checked) {
        emit gridEnable(checked);
    });
    m_glyphToolBar->addAction(m_gridEnable);

    m_userLayerEnable = new QAction(QIcon(":/button/icons/user"), "User Layer Enable", this);
    m_userLayerEnable->setCheckable(true);
    QObject::connect(m_userLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit userLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_userLayerEnable);


    m_previewLayerEnable = new QAction(QIcon(":/button/icons/preview"), "Preview Layer Enable", this);
    m_previewLayerEnable->setCheckable(true);
    QObject::connect(m_previewLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit previewLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_previewLayerEnable);

    m_glyphRectLayerEnable = new QAction(QIcon(":/button/icons/glyphrect"), "Glyph Rect Layer Enable", this);
    m_glyphRectLayerEnable->setCheckable(true);
    QObject::connect(m_glyphRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit glyphRectLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_glyphRectLayerEnable);


    m_bitmapRectLayerEnable = new QAction(QIcon(":/button/icons/bitmaprect"), "BaseLine Layer Enable", this);
    m_bitmapRectLayerEnable->setCheckable(true);
    QObject::connect(m_bitmapRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit bitmapRectLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_bitmapRectLayerEnable);

    m_baselineLayerEnable = new QAction(QIcon(":/button/icons/baseline"), "Bitmap Rect Layer Enable", this);
    m_baselineLayerEnable->setCheckable(true);
    QObject::connect(m_baselineLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit baselineLayerEnable(checked);
    });
    m_glyphToolBar->addAction(m_baselineLayerEnable);

    m_pasteGlyphToUserLayer = new QAction(QIcon(":/button/icons/paste"), "Paste Glyph to User Layer", this);
    QObject::connect(m_pasteGlyphToUserLayer, &QAction::triggered, this, [=](){
        emit pasteGlyphToUserLayer();
    });
    m_glyphToolBar->addAction(m_pasteGlyphToUserLayer);

    m_clearUserLayer = new QAction(QIcon(":/button/icons/clear"), "Clear User Layer");
    QObject::connect(m_clearUserLayer, &QAction::triggered, this, [=](){
        emit clearUserLayer();
    });
    m_glyphToolBar->addAction(m_clearUserLayer);


    m_addBottomCells = new QSpinBox(this);
    m_addBottomCells->setMinimum(0);
    m_addBottomCells->setMaximum(1240);
    QObject::connect(m_addBottomCells, &QSpinBox::valueChanged, this, [=](int value){
        emit leftGridCells(value);
    });
    m_glyphToolBar->addWidget(m_addBottomCells);

    m_addLeftCells = new QSpinBox(this);
    m_addLeftCells->setMinimum(0);
    m_addLeftCells->setMaximum(1240);
    QObject::connect(m_addLeftCells, &QSpinBox::valueChanged, this, [=](int value){
        emit bottomGridCells(value);
    });
    m_glyphToolBar->addWidget(m_addLeftCells);

    addToolBar(m_glyphToolBar);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Сохранение геометрии перед закрытием
    saveGeometryAndState();
    emit saveCurrentState();
    QMainWindow::closeEvent(event);
}

void MainWindow::setupStatusBar () {
    m_fontLabel = new QLabel(ui->statusbar);
    m_charLabel = new QLabel(ui->statusbar);
    m_glyphSizeLabel = new QLabel(ui->statusbar);
    m_gridSizeLabel = new QLabel(ui->statusbar);
    statusBar()->addPermanentWidget(m_fontLabel);
    statusBar()->addPermanentWidget(m_charLabel);
    statusBar()->addPermanentWidget(m_glyphSizeLabel);
    statusBar()->addPermanentWidget(m_gridSizeLabel);
}

void MainWindow::slotActionQuitTriggered()
{
    qApp->quit();
}

void MainWindow::setStatusBarFontName(const QFont &newFont)
{
    if (m_fontLabel != nullptr)
    {
        m_fontLabel->setText(QString("Font family: %1").arg(newFont.family()));
    }
}

void MainWindow::setStatusBarCharacter(const QChar &newValue)
{
    if (m_charLabel != nullptr)
    {
        m_charLabel->setText(QString("Character: '%1'").arg(newValue));
    }
}

void MainWindow::setStatusBarGlyphSize(int newValue)
{
    if (m_glyphSizeLabel != nullptr)
    {
        m_glyphSizeLabel->setText(QString("Glyph Size: %1 px").arg(newValue));
    }
}

void MainWindow::setStatusBarBitmapDimension (int newValue)
{
    if (m_gridSizeLabel != nullptr)
    {
        m_gridSizeLabel->setText(QString("Bitmap Size: %1x%1 px").arg(newValue));
    }
}

void MainWindow::saveGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.setValue("MainWindow/templateLayerEnable", m_templateLayerEnable->isChecked());
    settings.setValue("MainWindow/gridEnable", m_gridEnable->isChecked());
    settings.setValue("MainWindow/previewLayerEnable", m_previewLayerEnable->isChecked());
    settings.setValue("MainWindow/userLayerEnable", m_userLayerEnable->isChecked());
    settings.setValue("MainWindow/glyphRectLayerEnable", m_glyphRectLayerEnable->isChecked());
    settings.setValue("MainWindow/baseLineLayerEnable", m_baselineLayerEnable->isChecked());
    settings.setValue("MainWindow/bitmapRectLayerEnable", m_bitmapRectLayerEnable->isChecked());
    settings.setValue("MainWindow/addLeftCells", m_addLeftCells->value());
    settings.setValue("MainWindow/addBottomCells", m_addBottomCells->value());
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    bool setTemplateGlyphEnable = settings.value("templateLayerEnable").toBool();
    qDebug() << "Set Glyph Editable" << setTemplateGlyphEnable;
    if (m_templateLayerEnable) {
        m_templateLayerEnable->setChecked(setTemplateGlyphEnable);
    }
    bool gridEnable = settings.value("MainWindow/gridEnable").toBool();
    if (m_gridEnable) {
        m_gridEnable->setChecked(gridEnable);
    }
    bool userLayerEnable = settings.value("MainWindow/userLayerEnable").toBool();
    if (m_userLayerEnable) {
        m_userLayerEnable->setChecked(userLayerEnable);
    }
    bool previewLayerEnable = settings.value("MainWindow/previewLayerEnable").toBool();
    if (m_previewLayerEnable) {
        m_previewLayerEnable->setChecked(previewLayerEnable);
    }
    bool glyphRectLayerEnable = settings.value("MainWindow/glyphRectLayerEnable").toBool();
    if (m_glyphRectLayerEnable) {
        m_glyphRectLayerEnable->setChecked(glyphRectLayerEnable);
    }
    bool baselineLayerEnable = settings.value("MainWindow/baselineLayerEnable").toBool();
    if (m_baselineLayerEnable) {
        m_baselineLayerEnable->setChecked(baselineLayerEnable);
    }
    bool bitmapRectLayerEnable = settings.value("MainWindow/bitmapRectLayerEnable").toBool();
    if (m_bitmapRectLayerEnable) {
        m_bitmapRectLayerEnable->setChecked(bitmapRectLayerEnable);
    }
    int addLeftCells = settings.value("MainWindow/addLeftCells").toInt();
    m_addLeftCells->setValue(addLeftCells);
    int addBottomCells = settings.value("MainWindow/addBottomCells").toInt();
    m_addBottomCells->setValue(addBottomCells);
}

void MainWindow::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}
