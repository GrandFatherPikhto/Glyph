#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include "appsettings.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "profilemanager.h"

AppSettings::AppSettings(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_fontManager(nullptr)
{
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &AppSettings::setupValues);
}

AppSettings::~AppSettings ()
{
    // qDebug() << __FILE__ << __LINE__ << "Destroy AppSettings class";
    saveAppSettings ();
}

void AppSettings::setupSignals ()
{
    QObject::connect(this, &AppSettings::changeGlyphProfile, this, &AppSettings::setGlyphProfile);
}

void AppSettings::setupValues()
{
    Q_ASSERT(m_appContext->fontManager() != nullptr && m_appContext->profileManager() != nullptr);
    m_fontManager = m_appContext->fontManager();
    m_profileManager = m_appContext->profileManager();

    initAppDataCatalog ();
    restoreAppSettings ();
    
    setupSignals ();
}

const GlyphProfile & AppSettings::glyphProfile() const
{
    return m_glyphProfile;
}

void AppSettings::setGlyphProfile(const GlyphProfile &profile) { 
    if (m_glyphProfile != profile)
    {
        m_glyphProfile = profile;
        // if (!profile.temporary())
        // {
        //     m_profileManager->insertOrReplaceProfile(profile);
        // }
        emit glyphProfileChanged(m_glyphProfile);
    }
}

const QString & AppSettings::initAppDataCatalog()
{
    // Получение пути к AppData/Local/<AppName>
    m_appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    // qDebug() << m_appDataPath;
    // Создание каталога, если его нет
    QDir dir(m_appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return m_appDataPath;
}

const QString & AppSettings::appDataPath() const
{
    return m_appDataPath;
}


void AppSettings::saveAppSettings()
{
    QSettings settings(this);

    settings.setValue("glyphProfile", m_glyphProfile);
}

void AppSettings::restoreAppSettings()
{
    QSettings settings(this);

    m_glyphProfile = settings.value("glyphProfile", defaultGlyphProfile()).value<GlyphProfile>();
    m_glyphProfile.setTemporary(true);
}

GlyphProfile AppSettings::defaultGlyphProfile()
{
    QFont font("Arial");

    if(m_fontManager->loadFont(font))
    {
        return GlyphProfile("", 12, 10, font, m_fontManager->fontPath(), true);
    }

    return GlyphProfile();
}
