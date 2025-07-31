#include "appcontext.h"
#include "glyphmanager.h"
#include "fontmanager.h"
#include "glyphimagemanager.h"
#include "unicodemetadata.h"
#include "appsettings.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_imageManager(nullptr)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_appSettings(nullptr)
{
    initValues ();
    restoreAppContext();
    setupSignals();
}

AppContext::~AppContext()
{
    saveAppContext();
}

void AppContext::initValues ()
{
    m_appSettings = new AppSettings(this);
    m_fontManager = new FontManager(this);
    m_imageManager = new GlyphImageManager(this);
    m_glyphManager = new GlyphManager(this);
    m_unicodeMetadata = new UnicodeMetadata (this);
}

void AppContext::setupSignals()
{
}

void AppContext::saveAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");
    
    settings.endGroup();
}

void AppContext::restoreAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");

    settings.endGroup();
}

