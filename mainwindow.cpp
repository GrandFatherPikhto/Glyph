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
    , m_fontLabel(nullptr)
    , m_glyphSizeLabel(nullptr)
    , m_gridSizeLabel(nullptr)
    , m_charLabel(nullptr)
    , m_mainLayout(nullptr)
    , m_glyphWidget(nullptr)
    , m_gridEnable(nullptr)
    , m_generatedGlyphEnable(nullptr)
    , m_glyphGrid(nullptr)
    , m_contourEnable(nullptr)
{
    ui->setupUi(this);
    m_glyphManager = new GlyphManager(this);

    setupGlyphWidget();
    setupGlyphDock();
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
    QObject::connect(m_dockGlyph, &DockGlyph::glyphChanged, m_glyphWidget, &GlyphWidget::setGlyph);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphChanged, this, [=](QSharedPointer<Glyph> glyph){
        setStatusBarFontName(glyph->font());
        setStatusBarCharacter(glyph->character());
        setStatusBarGlyphSize(glyph->glyphSize());
        setStatusBarGridSize(glyph->gridSize());
    });
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, this, &MainWindow::setStatusBarFontName);
    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, this, &MainWindow::setStatusBarCharacter);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphSizeChanged, this, &MainWindow::setStatusBarGlyphSize);
    QObject::connect(m_dockGlyph, &DockGlyph::gridSizeChanged, this, &MainWindow::setStatusBarGridSize);
    QObject::connect(this, &MainWindow::generatedGlyphEnabled, m_glyphWidget, &GlyphWidget::enableGeneratedGlyph);
    QObject::connect(this, &MainWindow::contourEnabled, m_glyphWidget, &GlyphWidget::enableContour);
    QObject::connect(this, &MainWindow::gridEnabled, m_glyphWidget, &GlyphWidget::enableGrid);
    QObject::connect(this, &MainWindow::glyphGridEnabled, m_glyphWidget, &GlyphWidget::enableGlyphGrid);
}

void MainWindow::setupGlyphWidget ()
{
    m_mainLayout = new QGridLayout(this);
    m_glyphWidget = new GlyphWidget(m_glyphManager, this);
    m_mainLayout->addWidget(m_glyphWidget);
    ui->centralwidget->setLayout(m_mainLayout);
}

void MainWindow::setupGlyphDock ()
{
    m_dockGlyph = new DockGlyph(m_glyphManager, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyph);
    // tabifiedDockWidgetActivated(m_dockGlyph);
}

void MainWindow::setupGlyphToolBar()
{
    m_glyphToolBar = new QToolBar("Glyph Toolbar", this);
    m_glyphToolBar->setObjectName("GlyphToolbar");
    // m_glyphToolBar->addAction(QIcon(":/glyphtoolbar/icons/editDisable"), "New");
    m_generatedGlyphEnable = new QAction(QIcon(":/button/icons/edit"), "Generated Glyph", this);
    m_generatedGlyphEnable->setCheckable(true);
    connect(m_generatedGlyphEnable, &QAction::toggled, this, [=](bool checked) {
        emit generatedGlyphEnabled(checked);
    });
    m_glyphToolBar->addAction(m_generatedGlyphEnable);

    m_gridEnable = new QAction(QIcon(":/button/icons/grid"), "Grid Enable", this);
    m_gridEnable->setCheckable(true);
    connect(m_gridEnable, &QAction::toggled, this, [=](bool checked) {
        emit gridEnabled(checked);
    });
    m_glyphToolBar->addAction(m_gridEnable);

    m_glyphGrid = new QAction(QIcon(":/button/icons/glyphgrid"), "Glyph Grid Enable", this);
    m_glyphGrid->setCheckable(true);
    connect(m_glyphGrid, &QAction::toggled, this, [=](bool checked) {
        emit glyphGridEnabled(checked);
    });
    m_glyphToolBar->addAction(m_glyphGrid);


    m_contourEnable = new QAction(QIcon(":/button/icons/contour"), "Countour Enable", this);
    m_contourEnable->setCheckable(true);
    connect(m_contourEnable, &QAction::toggled, this, [=](bool checked) {
        emit contourEnabled(checked);
    });
    m_glyphToolBar->addAction(m_contourEnable);

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

void MainWindow::on_action_Quit_triggered()
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

void MainWindow::setStatusBarCharacter(const QChar &newCharacter)
{
    if (m_charLabel != nullptr)
    {
        m_charLabel->setText(QString("Character: '%1'").arg(newCharacter));
    }
}

void MainWindow::setStatusBarGlyphSize(int newGlyphSize)
{
    if (m_glyphSizeLabel != nullptr)
    {
        m_glyphSizeLabel->setText(QString("Glyph Size: %1x%1 px").arg(newGlyphSize));
    }
}

void MainWindow::setStatusBarGridSize (int newGridSize)
{
    if (m_gridSizeLabel != nullptr)
    {
        m_gridSizeLabel->setText(QString("Grid Size: %1x%1 px").arg(newGridSize));
    }
}

void MainWindow::saveGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.setValue("generatedGlyphEnabled", m_generatedGlyphEnable->isChecked());
    settings.setValue("gridEnabled", m_gridEnable->isChecked());
    settings.setValue("contourEnabled", m_contourEnable->isChecked());
    settings.setValue("glyphGridEnabled", m_glyphGrid->isChecked());
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    bool setGlyphEditable = settings.value("generatedGlyphEnabled").toBool();
    qDebug() << "Set Glyph Editable" << setGlyphEditable;
    if (m_generatedGlyphEnable) {
        m_generatedGlyphEnable->setChecked(setGlyphEditable);
    }
    bool gridEnabled = settings.value("gridEnabled").toBool();
    if (m_gridEnable) {
        m_gridEnable->setChecked(gridEnabled);
    }
    bool glyphGridEnabled = settings.value("glyphGridEnabled").toBool();
    if (m_glyphGrid) {
        m_glyphGrid->setChecked(glyphGridEnabled);
    }
    bool contourEnabled = settings.value("contourEnabled").toBool();
    if (m_contourEnable) {
        m_contourEnable->setChecked(contourEnabled);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}
