#include <QSettings>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dockGlyph(new DockGlyph(this))
    , m_fontLabel(nullptr)
    , m_glyphSizeLabel(nullptr)
    , m_gridSizeLabel(nullptr)
    , m_charLabel(nullptr)

{
    ui->setupUi(this);

    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyph);
    // tabifiedDockWidgetActivated(m_dockGlyph);
    connectGlyphDockEvents();
    setupStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Сохранение геометрии перед закрытием
    saveGeometryAndState();
    emit saveCurrentState();
    QMainWindow::closeEvent(event);
}

void MainWindow::connectGlyphDockEvents()
{
    QObject::connect(m_dockGlyph, &DockGlyph::glyphParamsChanged, ui->glyphWidget, &GlyphWidget::setGlyphParams);
    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, ui->glyphWidget, &GlyphWidget::setCharacter);
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, ui->glyphWidget, &GlyphWidget::setFont);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphSizeChanged, ui->glyphWidget, &GlyphWidget::setGlyphSize);
    QObject::connect(m_dockGlyph, &DockGlyph::gridSizeChanged, ui->glyphWidget, &GlyphWidget::setGridSize);
    QObject::connect(m_dockGlyph, &DockGlyph::moveGlyphLeft, ui->glyphWidget, &GlyphWidget::moveGlyphLeft);
    QObject::connect(m_dockGlyph, &DockGlyph::moveGlyphTop, ui->glyphWidget, &GlyphWidget::moveGlyphTop);
    QObject::connect(m_dockGlyph, &DockGlyph::moveGlyphRight, ui->glyphWidget, &GlyphWidget::moveGlyphRight);
    QObject::connect(m_dockGlyph, &DockGlyph::moveGlyphDown, ui->glyphWidget, &GlyphWidget::moveGlyphDown);
    QObject::connect(m_dockGlyph, &DockGlyph::moveGlyphCenter, ui->glyphWidget, &GlyphWidget::moveGlyphCenter);
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
    QObject::connect(m_dockGlyph, &DockGlyph::glyphParamsChanged, this, [=](const QFont &newFont, const QString &newFontPath, int newGlyphSize, const QChar &newCharacter, int newGridSize){
        setStatusBarFontName(newFont);
        setStatusBarCharacter(newCharacter);
        setStatusBarGlyphSize(newGlyphSize);
        setStatusBarGridSize(newGridSize);
    });
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, this, &MainWindow::setStatusBarFontName);
    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, this, &MainWindow::setStatusBarCharacter);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphSizeChanged, this, &MainWindow::setStatusBarGlyphSize);
    QObject::connect(m_dockGlyph, &DockGlyph::gridSizeChanged, this, &MainWindow::setStatusBarGridSize);
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
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}



void MainWindow::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}
