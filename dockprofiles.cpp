#include <QPushButton>
#include <QSpinBox>

#include "dockprofiles.h"
#include "ui_dockprofiles.h"

#include "appcontext.h"
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
    QObject::connect(m_appSettings, &AppSettings::glyphProfileChanged, this, [=](const GlyphProfile &profile){
        // qDebug() << __FILE__ << __LINE__ << profile;
        if (m_glyphProfile != profile)
        {
            m_glyphProfile = profile;
            loadGlyphProfile();
        }
    });

    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, this, [=](int value){
        m_glyphProfile.setBitmapDimension(value);
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->lineEditProfileName, &QLineEdit::editingFinished, this, [=](){
        m_glyphProfile.setName(ui->lineEditProfileName->text());
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->spinBoxPaddingLeft, &QSpinBox::valueChanged, this, [=](int value){
        m_glyphProfile.setPaddingLeft(value);
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->spinBoxPaddingTop, &QSpinBox::valueChanged, this, [=](int value){
        m_glyphProfile.setPaddingTop(value);
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->spinBoxPaddingRight, &QSpinBox::valueChanged, this, [=](int value){
        m_glyphProfile.setPaddingRight(value);
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->spinBoxPaddingBottom, &QSpinBox::valueChanged, this, [=](int value){
        m_glyphProfile.setPaddingBottom(value);
        emit m_appSettings->changeGlyphProfile(m_glyphProfile);
    });

    QObject::connect(ui->pushButtonCreateProfile, &QPushButton::clicked, this, [=](){
        if (m_glyphProfile.isValid() && m_glyphProfile.temporary())
        {
            // qDebug() << __FILE__ << __LINE__ << m_glyphProfile;
            m_glyphProfile.setTemporary(false);
            emit m_appSettings->changeGlyphProfile(m_glyphProfile);
            updateProfilesTable();
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
    m_glyphProfile = m_appSettings->glyphProfile();
    m_profileManager = m_appContext->profileManager();

    loadGlyphProfile();

    m_profilesModel = new QSqlTableModel(this);

    updateProfilesTable();
    
    m_profilesModel->setHeaderData(0, Qt::Horizontal, "Название");
    m_profilesModel->setHeaderData(1, Qt::Horizontal, "Шрифт");
    m_profilesModel->setHeaderData(2, Qt::Horizontal, "Размер");

    ui->tableViewProfiles->setModel(m_profilesModel);
    ui->tableViewProfiles->resizeColumnsToContents();

}

void DockProfiles::updateProfilesTable()
{
    QString strSql = QString("SELECT name, font_family, bitmap_dimension FROM %1").arg(m_profileManager->tableName());
    m_profilesModel->setQuery(strSql);
}

void DockProfiles::loadGlyphProfile()
{
    ui->labelFont->setText(QString("Font: %1").arg(m_glyphProfile.font().family()));
    ui->spinBoxBitmapDimension->setValue(m_glyphProfile.bitmapDimension());
    if (m_glyphProfile.paddingLeft() != ui->spinBoxPaddingLeft->value())
    {
        ui->spinBoxPaddingLeft->setValue(m_glyphProfile.paddingLeft());
    }

    if (m_glyphProfile.paddingTop() != ui->spinBoxPaddingTop->value())
    {
        ui->spinBoxPaddingTop->setValue(m_glyphProfile.paddingTop());
    }

    if (m_glyphProfile.paddingRight() != ui->spinBoxPaddingRight->value())
    {
        ui->spinBoxPaddingRight->setValue(m_glyphProfile.paddingRight());
    }


    if (m_glyphProfile.paddingBottom() != ui->spinBoxPaddingBottom->value())
    {
        ui->spinBoxPaddingBottom->setValue(m_glyphProfile.paddingBottom());
    }

    if (ui->lineEditProfileName->text() != m_glyphProfile.name())
    {
        ui->lineEditProfileName->setText(m_glyphProfile.name());
    }
}
