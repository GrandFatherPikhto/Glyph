#include "appcontext.h"
#include "glyphmanager.h"
#include "fontmanager.h"
#include "glyphimagemanager.h"
#include "dimensionmanager.h"
#include "unicodemetadata.h"
#include "appsettings.h"
#include "glyphfilter.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_imageManager(nullptr)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_appSettings(nullptr)
    , m_glyphFilter(nullptr)
    , m_dimensionManager(nullptr)
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
    m_glyphFilter = new GlyphFilter (this);
    m_appSettings = new AppSettings (this);
    m_fontManager = new FontManager (this);
    m_imageManager = new GlyphImageManager (this);
    m_unicodeMetadata = new UnicodeMetadata (this);
    m_dimensionManager = new DimensionManager (this);
    m_glyphManager = new GlyphManager (this);
    
    m_glyphManager->setDimensionManager(m_dimensionManager);
    m_glyphManager->setFontManager(m_fontManager);
    m_glyphManager->setGlyphFilter(m_glyphFilter);
    m_glyphManager->setAppSettings(m_appSettings);

    m_imageManager->setAppSettings(m_appSettings);
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

