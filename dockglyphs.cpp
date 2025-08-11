#include "dockglyphs.h"
#include "ui_dockglyphs.h"

#include "appcontext.h"
#include "profilemanager.h"
#include "charmapmanager.h"
#include "appsettings.h"

DockGlyphs::DockGlyphs(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphs)
    , m_appContext(appContext)
    , m_appSettings(nullptr)
    , m_profileManager(nullptr)
    , m_charmapManager(nullptr)
    , m_profilesModel(nullptr)
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
    Q_ASSERT(m_appContext->appSettings() != nullptr);
    m_appSettings = m_appContext->appSettings();
    m_profilesModel = new QSqlQueryModel(this);
    refreshProfilesComboBox();
}

void DockGlyphs::refreshProfilesComboBox()
{
    QString sql = "SELECT "
                  "id, "
                  "name || ' (' || font_family || ', ' || bitmap_dimension || 'px)' AS display_name, "
                  "name, bitmap_dimension, temporary, font_family, font_path, padding_left, padding_top, padding_right, padding_bottom "
                  "FROM profiles  "
                  "ORDER BY name, bitmap_dimension DESC";
    m_profilesModel->setQuery(sql);
    ui->comboBoxProfiles->setModel(m_profilesModel);
    ui->comboBoxProfiles->setModelColumn(1);
}

void DockGlyphs::setupSignals()
{
    QObject::connect(m_appSettings, &AppSettings::glyphProfileChanged, this, [=](const GlyphProfile &profile){
        m_glyphProfile = profile;
    });

    QObject::connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &DockGlyphs::setCurrentProfile);
}

void DockGlyphs::setCurrentProfile(int idx)
{
    if (idx < 0)
        return;

    QModelIndex fieldIdx = m_profilesModel->index(idx, 2);

    fieldIdx = m_profilesModel->index(idx, 0);
    int id = m_profilesModel->data(fieldIdx).toInt();
    GlyphProfile profile;
    m_profileManager->getProfileById(id, profile);

    qDebug() << __FILE__ << __LINE__ << profile;
}
