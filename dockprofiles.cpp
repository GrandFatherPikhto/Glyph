#include <QPushButton>
#include <QSpinBox>

#include "dockprofiles.h"
#include "ui_dockprofiles.h"

#include "appcontext.h"
#include "profilemanager.h"
#include "appsettings.h"
#include "charmapmanager.h"
#include "profilemanager.h"

DockProfiles::DockProfiles(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockProfiles)
    , m_appContext(appContext)
    , m_appSettings(nullptr)
    , m_profileManager(nullptr)
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
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &profile){
        // qDebug() << __FILE__ << __LINE__ << profile;
        if (m_profile != profile)
        {
            m_profile = profile;
            loadProfileContext();
            emit m_profileManager->changeProfile(profile);
        }
    });

    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setBitmapDimension(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->lineEditProfileName, &QLineEdit::editingFinished, this, [=](){
        m_profile.setName(ui->lineEditProfileName->text());
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->spinBoxPaddingLeft, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setPaddingLeft(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->spinBoxPaddingTop, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setPaddingTop(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->spinBoxPaddingRight, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setPaddingRight(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->spinBoxPaddingBottom, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setPaddingBottom(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, this, [=](int value){
        m_profile.setGlyphSize(value);
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->pushButtonCreateProfile, &QPushButton::clicked, this, [=](){
        if (m_profile.isValid() && m_profile.temporary())
        {
            // qDebug() << __FILE__ << __LINE__ << m_profile;
            m_profile.setTemporary(false);
            if (m_profileManager->insertOrReplaceProfile(m_profile))
            {
                emit m_profileManager->changeProfile(m_profile);
                updateProfilesTable();
            }
        }
    });

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

    QObject::connect(m_profileManager, &ProfileManager::profilesChanged, this, [=](){
        updateProfilesTable();
    });
}

void DockProfiles::setupValues()
{
    Q_ASSERT(m_appContext->appSettings());

    m_appSettings = m_appContext->appSettings();
    m_profileManager = m_appContext->profileManager();
    
    m_profile = m_profileManager->profile();

    loadProfileContext();

    m_profilesModel = new QSqlTableModel(this);

    updateProfilesTable();
    
    ui->comboBoxProfiles->setModel(m_profilesModel);
    ui->comboBoxProfiles->setModelColumn(1);
}

void DockProfiles::updateProfilesTable()
{
    // QString strSql = QString("SELECT name, font_family, bitmap_dimension FROM %1").arg(m_profileManager->tableName());
    QString strSql = "SELECT "
                "id, "
                "name || ' (' || font_family || ', ' || bitmap_dimension || 'px)' AS display_name, "
                "name, bitmap_dimension, temporary, font_family, font_path, padding_left, padding_top, padding_right, padding_bottom "
                "FROM profiles  "
                "ORDER BY name, bitmap_dimension DESC";
    m_profilesModel->setQuery(strSql);
}

void DockProfiles::loadProfileContext()
{
    ui->labelFont->setText(QString("Font: %1").arg(m_profile.font().family()));
    ui->spinBoxBitmapDimension->setValue(m_profile.bitmapDimension());
    if (m_profile.paddingLeft() != ui->spinBoxPaddingLeft->value())
    {
        ui->spinBoxPaddingLeft->setValue(m_profile.paddingLeft());
    }

    if (m_profile.paddingTop() != ui->spinBoxPaddingTop->value())
    {
        ui->spinBoxPaddingTop->setValue(m_profile.paddingTop());
    }

    if (m_profile.paddingRight() != ui->spinBoxPaddingRight->value())
    {
        ui->spinBoxPaddingRight->setValue(m_profile.paddingRight());
    }


    if (m_profile.paddingBottom() != ui->spinBoxPaddingBottom->value())
    {
        ui->spinBoxPaddingBottom->setValue(m_profile.paddingBottom());
    }

    if (ui->lineEditProfileName->text() != m_profile.name())
    {
        ui->lineEditProfileName->setText(m_profile.name());
    }
}
