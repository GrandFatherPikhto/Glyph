#include <QPushButton>
#include <QSpinBox>

#include "dockprofiles.h"
#include "ui_dockprofiles.h"

#include "appcontext.h"
#include "profilemanager.h"
#include "fontmanager.h"
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
        m_profileManager->defaultProfile(m_profile);
        qDebug() << __FILE__ << __LINE__ << m_profile;
        if (m_profile.isValid() && m_profile.id() < 0)
        {
            if (m_profileManager->insertOrReplaceProfile(m_profile))
            {
                emit m_profileManager->changeProfile(m_profile);
                updateProfilesCombo();
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
        updateProfilesCombo();
    });
}

void DockProfiles::setupValues()
{
    Q_ASSERT(m_appContext->appSettings());

    m_profile = m_profileManager->profile();
    m_fontManager->fontContextById(m_profile.fontId(), m_font);

    loadProfileContext();

    m_profilesModel = new QSqlQueryModel(this);

    updateProfilesCombo();
    
    ui->comboBoxProfiles->setModel(m_profilesModel);
    ui->comboBoxProfiles->setModelColumn(1);
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
    m_fontManager->fontContextById(m_profile.fontId(), m_font);
    ui->labelFont->setText(QString("Font: %1/%2").arg(m_font.name(), m_font.family()));
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
