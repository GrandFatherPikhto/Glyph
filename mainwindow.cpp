#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dockGlyph(new DockGlyph(this))
{
    ui->setupUi(this);

    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyph);
    // tabifiedDockWidgetActivated(m_dockGlyph);

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

