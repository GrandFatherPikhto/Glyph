#include <QPushButton>
#include <QSpinBox>

#include "dockprofiles.h"
#include "ui_dockprofiles.h"

#include "glyphcontext.h"
#include "appcontext.h"
#include "profilemanager.h"
#include "fontmanager.h"
#include "glyphmanager.h"
#include "appsettings.h"
#include "charmapmanager.h"
#include "profilemanager.h"
#include "fontmanager.h"

DockProfiles::DockProfiles(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockProfiles)
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_fontManager(appContext->fontManager())
    , m_profileManager(appContext->profileManager())
    , m_glyphManager(appContext->glyphManager())
    , m_profilesModel(nullptr)
{
    ui->setupUi(this);
    setupValues();
    setupSignals ();
}

DockProfiles::~DockProfiles()
{
    delete ui;
}

void DockProfiles::setupSignals()
{
    

    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &value) {
        loadProfileContext();
    });

    QObject::connect(ui->spinBoxWidth, &QSpinBox::valueChanged, this, [=](int value) {
        ProfileContext profile = m_profileManager->profile();
        profile.setGridWidth(value);
        m_appSettings->setValue("defaultGridWidth", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxHeight, &QSpinBox::valueChanged, this, [=](int value) {
        ProfileContext profile = m_profileManager->profile();
        profile.setGridHeight(value);
        m_appSettings->setValue("defaultGridHeight", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->lineEditProfileName, &QLineEdit::editingFinished, this, [=]() {
        ProfileContext profile = m_profileManager->profile();
        profile.setName(ui->lineEditProfileName->text());
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxPaddingLeft, &QSpinBox::valueChanged, this, [=](int value) {
        ProfileContext profile = m_profileManager->profile();
        profile.setGridLeft(value);
        m_appSettings->setValue("defaultGridLeft", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxPaddingTop, &QSpinBox::valueChanged, this, [=](int value) {
        ProfileContext profile = m_profileManager->profile();
        profile.setGridTop(value);
        m_appSettings->setValue("defaultGridTop", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxPaddingRight, &QSpinBox::valueChanged, this, [=](int value) {
        ProfileContext profile = m_profileManager->profile();
        profile.setGridRight(value);
        m_appSettings->setValue("defaultGridRight", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxPaddingBottom, &QSpinBox::valueChanged, this, [=](int value){
        ProfileContext profile = m_profileManager->profile();
        profile.setGridBottom(value);
        m_appSettings->setValue("defaultGridBottom", value);
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, this, [=](int value){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.setSize(value);
        m_appSettings->setValue("defaultGlyphSize", value);
        emit m_glyphManager->changeGlyph(glyph);
    });

    QObject::connect(ui->pushButtonCreateProfile, &QPushButton::clicked, this, [=](){
        ProfileContext profile = m_profileManager->profile();
        // m_profileManager->defaultProfile(profile);
        qDebug() << __FILE__ << __LINE__ << profile;
        if (profile.isValid() && profile.id() < 0)
        {
            if (m_profileManager->insertOrReplaceProfile(profile))
            {
                emit m_profileManager->changeProfile(profile);
                updateProfilesCombo();
            }
        }
    });

/*
    QObject::connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, [=](int value){
        QModelIndex idIdx = m_profilesModel->index(value, 0);
        if (idIdx.isValid())
        {
            int id = m_profilesModel->data(idIdx, Qt::DisplayRole).toInt();
            ProfileContext profile;
            if (m_profileManager->getProfileById(id, profile))
            {
                // qDebug() << __FILE__ << __LINE__ << "Profile Id:" << id << ", Profile: " << profile;
                m_profile = profile;
                emit m_profileManager->changeProfile(profile);
            }
        }
    });
*/
    QObject::connect(m_profileManager, &ProfileManager::profilesChanged, this, [=](){
        updateProfilesCombo();
    });
}

void DockProfiles::setupValues()
{
    Q_ASSERT(m_appContext->appSettings());

    // m_fontManager->fontContextById(profile.fontId(), m_font);

    loadProfileContext();

    m_profilesModel = new QSqlQueryModel(this);

    updateProfilesCombo();
    
    // ui->comboBoxProfiles->setModel(m_profilesModel);
    // ui->comboBoxProfiles->setModelColumn(1);
}

void DockProfiles::updateProfilesCombo()
{
    QString strSql = QString(
    "SELECT p.id, p.name || ' (' || f.family || ', ' || p.bitmap_dimension || 'px)' AS display_name, "
    "p.name, p.bitmap_dimension, p.padding_left, p.padding_top, p.padding_right, p.padding_bottom "
    "FROM  %1 p JOIN  %2 f ON f.id = p.font_id "
    " ORDER BY p.name, p.bitmap_dimension DESC;").arg(m_profileManager->tableName(), m_fontManager->tableName());
    m_profilesModel->setQuery(strSql, QSqlDatabase::database("main"));
}

void DockProfiles::loadProfileContext()
{
    ProfileContext profile = m_profileManager->profile();

    m_fontManager->fontContextById(profile.fontId(), m_font);
    // ui->labelFont->setText(QString("Font: %1/%2").arg(m_font.name(), m_font.family()));
    // ui->spinBoxBitmapDimension->setValue(m_profile.bitmapDimension());
    if (profile.gridLeft() != ui->spinBoxPaddingLeft->value())
    {
        ui->spinBoxPaddingLeft->setValue(profile.gridLeft());
    }

    if (profile.gridWidth() != ui->spinBoxWidth->value())
    {
        ui->spinBoxWidth->setValue(profile.gridWidth());
    }

    if (profile.gridTop() != ui->spinBoxPaddingTop->value())
    {
        ui->spinBoxPaddingTop->setValue(profile.gridTop());
    }

    if (profile.gridRight() != ui->spinBoxPaddingRight->value())
    {
        ui->spinBoxPaddingRight->setValue(profile.gridRight());
    }

    if (profile.gridHeight() != ui->spinBoxHeight->value())
    {
        ui->spinBoxHeight->setValue(profile.gridHeight());
    }

    if (profile.gridBottom() != ui->spinBoxPaddingBottom->value())
    {
        ui->spinBoxPaddingBottom->setValue(profile.gridBottom());
    }

    if (profile.glyphSize() != ui->spinBoxGlyphSize->value())
    {
        ui->spinBoxGlyphSize->setValue(profile.glyphSize());
    }

    if (ui->lineEditProfileName->text() != profile.name())
    {
        ui->lineEditProfileName->setText(profile.name());
    }
}
