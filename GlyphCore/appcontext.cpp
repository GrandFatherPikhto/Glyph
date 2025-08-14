#include "appcontext.h"
#include "dbcore.h"
#include "fontmanager.h"
#include "charmapmanager.h"
#include "unicodemetadata.h"
#include "profilemanager.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "imagemanager.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_dbCore(nullptr)
    , m_fontManager(nullptr)
    , m_charmapManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_profileManager(nullptr)
    , m_appSettings(nullptr)
    , m_glyphManager(nullptr)
    , m_imageManager(nullptr)
{
    setupVariables();
}

AppContext::~AppContext()
{
    // qDebug() << __FILE__ << __LINE__ << "Destroy AppContext class";
}

void AppContext::setupVariables()
{
    m_appSettings = new AppSettings(this);
    m_fontManager = new FontManager(this);
    m_dbCore = new DbCore(this);
    m_profileManager = new ProfileManager(this);
    m_charmapManager = new CharmapManager(this);
    m_unicodeMetadata = new UnicodeMetadata(this);
    m_glyphManager = new GlyphManager(this);
    m_imageManager = new ImageManager(this);

    emit valuesInited ();
}
