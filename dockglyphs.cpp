#include "dockglyphs.h"
#include "ui_dockglyphs.h"

#include "appcontext.h"
#include "profilemanager.h"
#include "charmapmanager.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "glyphmodel.h"

DockGlyphs::DockGlyphs(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphs)
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_glyphManager(appContext->glyphManager())
    , m_profileManager(appContext->profileManager())
    , m_charmapManager(appContext->charmapManager())
    , m_glyphsModel(nullptr)
{
    ui->setupUi(this);
    setupValues ();
    setupSignals ();
}

DockGlyphs::~DockGlyphs()
{
    delete ui;
}

void DockGlyphs::setupValues()
{
    m_glyphsModel = new GlyphModel(m_appContext, this);
    m_profile = m_profileManager->profile();

    ui->tableViewGlyphs->setModel(m_glyphsModel);

    ui->spinBoxGlyphSize->setValue(m_glyphManager->glyph().size());

    refreshGlyphsTable ();
}

void DockGlyphs::refreshGlyphsTable ()
{
    QSqlQuery query(QSqlDatabase::database("main"));
    m_glyphManager->queryGlyphsByProfile(query, m_profileManager->profile());
    m_glyphsModel->setQuery(std::move(query));
}

void DockGlyphs::setupSignals()
{
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &profile){
        m_profile = profile;
        refreshGlyphsTable();
    });

    QObject::connect(m_glyphManager, &GlyphManager::glyphsTableChanged, this, [=](const ProfileContext &profile) {
        if (profile != m_profile)
        {
            refreshGlyphsTable();
        }
    });

    QObject::connect(m_glyphManager, &GlyphManager::glyphAppended, this, [=](const GlyphContext &context){
        refreshGlyphsTable();
    });

    QObject::connect(m_glyphManager, &GlyphManager::glyphRemoved, this, [=](const GlyphContext &context){
        refreshGlyphsTable();
    });

    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, [=](const GlyphContext &context){
        ui->spinBoxGlyphSize->setValue(context.size());
    });
}

void DockGlyphs::setProfile()
{
    m_profile = m_profileManager->profile();
    m_glyph = m_glyphManager->glyph();
    ui->spinBoxGlyphSize->setValue(m_glyph.size());
}

void DockGlyphs::saveDockGlyphsSettings()
{
    QSettings settings(this);
    settings.beginGroup("DockGlyphs");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitterState", ui->splitter->saveState());
    settings.setValue("splitterGeometry", ui->splitter->saveGeometry());
    settings.endGroup();
}

void DockGlyphs::restoreDockGlyphsSettings()
{
    QSettings settings(this);
    settings.beginGroup("DockGlyphs");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitterState").toByteArray());
    ui->splitter->restoreGeometry(settings.value("splitterGeometry").toByteArray());
    settings.endGroup();
}

void DockGlyphs::showEvent(QShowEvent *event)
{
    restoreDockGlyphsSettings();
}

void DockGlyphs::hideEvent(QHideEvent *event)
{
    saveDockGlyphsSettings();
}

void DockGlyphs::closeEvent(QCloseEvent *event)
{
    saveDockGlyphsSettings();
}

