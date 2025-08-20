#include "appcontext.h"
#include "fontmanager.h"
#include "charmapmanager.h"
#include "unicodemetadata.h"
#include "profilemanager.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "imagemanager.h"
#include "dbmanager.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_appSettings(nullptr)
    , m_fontManager(nullptr)
    , m_charmapManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_profileManager(nullptr)
    , m_glyphManager(nullptr)
    , m_imageManager(nullptr)
    , m_dbManager(nullptr)
{
    initAppUserDir();
    setupVariables();
}

AppContext::~AppContext()
{

}

void AppContext::setupVariables()
{
    // dbManager();
    unicodeMetadata();
}

AppSettings* AppContext::appSettings()
{
    if (m_appSettings == nullptr)
    {
        m_appSettings = new AppSettings(this);
    }

    return m_appSettings; 
}

FontManager * AppContext::fontManager()
{ 
    if (m_fontManager == nullptr)
    {
        m_fontManager = new FontManager(this);
    }

    return m_fontManager; 
}

CharmapManager * AppContext::charmapManager()
{
    if (m_charmapManager == nullptr)
    {
        m_charmapManager = new CharmapManager(this);
    }

    return m_charmapManager;
}

UnicodeMetadata * AppContext::unicodeMetadata()
{ 
    if (m_unicodeMetadata == nullptr)
    {
        m_unicodeMetadata = new UnicodeMetadata(this);
    }

    return m_unicodeMetadata;
}

ProfileManager * AppContext::profileManager()
{
    if (m_profileManager == nullptr)
    {
        m_profileManager = new ProfileManager(this);
    }

    return m_profileManager;
}

GlyphManager * AppContext::glyphManager()
{
    if (m_glyphManager == nullptr)
    {
        m_glyphManager = new GlyphManager(this);
    }

    return m_glyphManager;
}

ImageManager * AppContext::imageManager()
{
    if (m_imageManager == nullptr)
    {
        m_imageManager = new ImageManager(this);
    }

    return m_imageManager;
}

DbManager * AppContext::dbManager()
{
    if (m_dbManager == nullptr)
    {
        m_dbManager = new DbManager(this);
    }

    return m_dbManager;
}

void AppContext::initAppUserDir()
{
    m_appUserDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(m_appUserDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}
