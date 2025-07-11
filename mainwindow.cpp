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

    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, this, [=](const QChar &character){
        qDebug() << character;
    });
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, this, [=](const QFont &newFont, const QString &fontPath){
        qDebug() << newFont << fontPath;
    });

    QObject::connect(m_dockGlyph, &DockGlyph::glyphParamsChanged, ui->glyphWidget, &GlyphWidget::setGlyphParams);
    QObject::connect(m_dockGlyph, &DockGlyph::characterChanged, ui->glyphWidget, &GlyphWidget::setCharacter);
    QObject::connect(m_dockGlyph, &DockGlyph::fontChanged, ui->glyphWidget, &GlyphWidget::setFont);
    QObject::connect(m_dockGlyph, &DockGlyph::glyphSizeChanged, ui->glyphWidget, &GlyphWidget::setGlyphSize);
    QObject::connect(m_dockGlyph, &DockGlyph::gridSizeChanged, ui->glyphWidget, &GlyphWidget::setGridSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

