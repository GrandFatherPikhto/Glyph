#include "dockglyphs.h"
#include "ui_dockglyphs.h"

#include "appcontext.h"
#include "profilemanager.h"
#include "fontmanager.h"
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
    , m_fontManager(appContext->fontManager())
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
    refreshGlyphsTable ();
    m_profile = m_profileManager->profile();

    ui->tableViewGlyphs->setModel(m_glyphsModel);

    ui->spinBoxGlyphSize->setValue(m_glyphManager->glyph().size());

    m_profilesModel = new QSqlQueryModel(this);
    refreshProfilesCombo();
    ui->comboBoxProfile->setModel(m_profilesModel);
    ui->comboBoxProfile->setModelColumn(1);
}

void DockGlyphs::refreshGlyphsTable ()
{
    QSqlQuery query(QSqlDatabase::database("main"));
    m_glyphManager->queryGlyphsByProfile(query, m_profileManager->profile());
    m_glyphsModel->setQuery(std::move(query));
}

void DockGlyphs::refreshProfilesCombo ()
{
    QString strSql = QString(
        "SELECT pr.id, pr.name || ' (' || fn.family || ', ' || pr.glyph_size || 'px)' AS display_name, "
        "pr.name, pr.glyph_size "
        "FROM  %1 pr JOIN  %2 fn ON fn.id = pr.font_id "
        "ORDER BY pr.name, pr.glyph_size ASC").arg(m_profileManager->tableName(), m_fontManager->tableName());
    QSqlQuery query(QSqlDatabase::database("main"));

    if (query.prepare(strSql))
    {
        if (query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastQuery();
            m_profilesModel->setQuery(std::move(query));
        }
    } else
    {
        qWarning() << __FILE__ << __LINE__ << "Error fetch profiles" << query.lastQuery() << ", Error: " << query.lastError();
    }
}

void DockGlyphs::setupSignals()
{
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &profile){
        m_profile = profile;
        refreshGlyphsTable();
    });

    connect(m_profileManager, &ProfileManager::profilesChanged, this, [=]{
        refreshProfilesCombo();
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
        ProfileContext profile = m_profileManager->profile();
        // qDebug() << __FILE__ << __LINE__ << context << profile;
        ui->spinBoxGlyphSize->setValue(context.size());
    });

    connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, this, [=](int value){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.setSize(value);
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->pushButtonLeft, &QPushButton::clicked, this, [=](){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.moveLeft();
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->pushButtonUp, &QPushButton::clicked, this, [=](){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.moveUp();
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->pushButtonRight, &QPushButton::clicked, this, [=](){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.moveRight();
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->pushButtonDown, &QPushButton::clicked, this, [=](){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.moveDown();
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->pushButtonReset, &QPushButton::clicked, this, [=](){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.resetOffset();
        emit m_glyphManager->changeGlyph(glyph);
    });

    connect(ui->comboBoxProfile, &QComboBox::currentIndexChanged, this, [=](int row){
        QModelIndex idIndex = m_profilesModel->index(row, 0);
        int profileId = m_profilesModel->data(idIndex).toInt();
        // qDebug() << __FILE__ << __LINE__ << row << profileId;
        ProfileContext profile;
        if (m_profileManager->getProfileById(profileId, profile))
        {
            emit m_profileManager->changeProfile(profile);
        }
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

