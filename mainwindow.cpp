#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QLayout>
#include <QDir>

#include "dockcharmap.h"
#include "dockglyphs.h"
#include "appcontext.h"
#include "dockprofiles.h"
#include "glyphdraw.h"
#include "dbcore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_appContext(nullptr)
    , m_dockCharmap(nullptr)
    , m_dockProfiles(nullptr)
    , m_dockGlyphs(nullptr)
{
    ui->setupUi(this);

    setupValues();
    setupSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSignals()
{
    QObject::connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::quitApplication);
    QObject::connect(ui->action_New_project, &QAction::triggered, this, &MainWindow::createProject);
}

void MainWindow::quitApplication()
{
    qApp->quit();
}

void MainWindow::createProject()
{
    QString filePath =
        QFileDialog::getSaveFileName(
            nullptr,
            "Создать новую базу данных SQLite",
            QDir::homePath(),
            "SQLite Databases (*.sqlite);;All Files (*)"
            );
    qDebug() << filePath;
    if (filePath.isEmpty())
        return;

    m_appContext->dbCore()->createDatabase(filePath);
}

void MainWindow::setupValues()
{
    m_appContext = new AppContext(this);
    ui->widgetGlyphDraw->setAppContext(m_appContext);

    m_dockCharmap = new DockCharmap(m_appContext, this);
    m_dockProfiles = new DockProfiles(m_appContext, this);
    m_dockGlyphs = new DockGlyphs(m_appContext, this);

    addDockWidget(Qt::LeftDockWidgetArea, m_dockCharmap);
    addDockWidget(Qt::RightDockWidgetArea, m_dockProfiles);
    addDockWidget(Qt::RightDockWidgetArea, m_dockGlyphs);
    tabifyDockWidget(m_dockProfiles, m_dockGlyphs);
}

void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    restoreGeometryAndState ();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    saveGeometryAndState ();
}

void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    saveGeometryAndState ();
}


void MainWindow::saveGeometryAndState() {
    QSettings settings(this);
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    // m_appContext->appSettings()->saveApplicationSettings();
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings(this);
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    // m_appContext->appSettings()->restoreApplicationSettings();
}

