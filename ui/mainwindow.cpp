#include <QSettings>
#include <QSharedPointer>
#include <QToolButton>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "glyphwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_appContext(nullptr)
    , m_dockGlyph(nullptr)
    , m_dockGlyphSelector(nullptr)
    , m_centralLayout(nullptr)
    , m_glyphWidget(nullptr)
    , m_mainToolbar(nullptr)
    , m_mainStatubar(nullptr)
{
    ui->setupUi(this);
    m_appContext = new AppContext(this);

    setupMainToolbar ();
    setupGlyphWidget();
    setupDockPanels();
    setupStatusBar();
    setupSignals ();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSignals ()
{
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::slotActionQuitTriggered);
    QObject::connect(m_appContext, &AppContext::glyphChanged, m_glyphWidget, &GlyphWidget::setGlyphMeta);
    QObject::connect(m_appContext, &AppContext::glyphChanged, this, [=](QSharedPointer<GlyphMeta> glyphMeta){
        // qDebug() << __FILE__ << __LINE__ << glyphMeta->toString();
    });


#if 0    
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
#endif
    //QObject::connect(m_dockGlyphSelector, &DockGlyphSelector::glyphChanged, m_glyphWidget, &GlyphWidget::setGlyphMeta);
}

void MainWindow::setupGlyphWidget ()
{
    m_glyphWidget = new GlyphWidget(m_appContext, this);
    setCentralWidget(m_glyphWidget);
}

void MainWindow::setupDockPanels ()
{
    m_dockGlyph = new DockGlyph(m_appContext, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyph);
    m_dockGlyphSelector = new DockGlyphSelector(m_appContext, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyphSelector);
    // tabifiedDockWidgetActivated(m_dockGlyph, m_dockGlyphSelector);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Сохранение геометрии перед закрытием
    saveGeometryAndState();
    emit saveCurrentState();
    QMainWindow::closeEvent(event);
}

void MainWindow::setupMainToolbar ()
{
    m_mainToolbar = new MainToolbar(m_appContext, this);
    addToolBar(m_mainToolbar);
}

void MainWindow::setupStatusBar () {
    m_mainStatubar = new MainStatusbar(m_appContext, this);
    setStatusBar(nullptr);
    setStatusBar(m_mainStatubar);
}

void MainWindow::slotActionQuitTriggered()
{
    qApp->quit();
}

void MainWindow::saveGeometryAndState() {
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    // m_appContext->appSettings()->saveApplicationSettings();
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    // m_appContext->appSettings()->restoreApplicationSettings();
}

void MainWindow::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}
