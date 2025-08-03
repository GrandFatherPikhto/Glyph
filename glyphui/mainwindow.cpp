#include <QSettings>
#include <QSharedPointer>
#include <QToolButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "glyphwidget.h"
#include "dockglyphtable.h"
#include "dockglyphselector.h"
#include "dockglyphprofiles.h"
#include "appcontext.h"
#include "appproject.h"
#include "glyphwidget.h"
#include "maintoolbar.h"
#include "mainstatusbar.h"
#include "dockglyphedit.h"
#include "appsettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_appContext(nullptr)
    // , m_dockGlyphTable(nullptr)
    , m_dockGlyphSelector(nullptr)
    , m_dockGlyphEdit(nullptr)
    , m_centralLayout(nullptr)
    , m_glyphWidget(nullptr)
    , m_mainToolbar(nullptr)
    , m_mainStatubar(nullptr)
{
    ui->setupUi(this);
    m_appContext = new AppContext(this);
    m_appSettings = m_appContext->appSettings();

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
    QObject::connect(ui->action_New_Project, &QAction::triggered, this, [=](bool triggered){
        QString projectPath = createNewProject();
        qDebug() << projectPath;
    });
}

void MainWindow::setupGlyphWidget ()
{
    m_glyphWidget = new GlyphWidget(m_appContext, this);
    setCentralWidget(m_glyphWidget);
}

void MainWindow::setupDockPanels ()
{
    m_dockGlyphSelector = new DockGlyphSelector(m_appContext, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyphSelector);
    // m_dockGlyphTable = new DockGlyphTable(m_appContext, this);
    // addDockWidget(Qt::RightDockWidgetArea, m_dockGlyphTable);
    m_dockGlyphEdit = new DockGlyphEdit(m_appContext, this);
    addDockWidget(Qt::RightDockWidgetArea, m_dockGlyphEdit);
    // emit tabifyDockWidget(m_dockGlyphTable, m_dockGlyphEdit);
    // emit tabifiedDockWidgetActivated(m_dockGlyphTable);
    m_dockGlyphProfiles = new DockGlyphProfiles(m_appContext, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockGlyphProfiles);
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

QString MainWindow::createNewProject()
{
    // Используем getSaveFileName для возможности ввода нового пути
    QFileDialog dialog(this);
    dialog.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dirPath = QFileDialog::getSaveFileName(
        this,
        "Select new Project Path",
        m_appSettings->defaultProjectPath(),
        "",
        nullptr,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (dirPath.isEmpty())
        return QString ();

    QDir dir(dirPath);
    QString projectDirectoryName = dir.dirName();
    if (!dir.exists())
    {
        AppProject *appProject = m_appContext->appProject();
        appProject->createProject(dirPath);
    }

    return dirPath;
}
