#include "appcontext.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_fontManager(nullptr)
    , m_appSettings(nullptr)
    , m_unicodeMetadata(nullptr)
{
    m_fontManager = new FontManager(this);
    m_appSettings = new AppSettings(m_fontManager, this);
    m_glyphManager = new GlyphManager(m_appSettings, this);
    m_unicodeMetadata = new UnicodeMetadata (this);
}

AppContext::~AppContext()
{

}
