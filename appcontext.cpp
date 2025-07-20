#include "appcontext.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_fontManager(nullptr)
    , m_appSettings(nullptr)
{
    m_fontManager = new FontManager(parent);
    m_appSettings = new AppSettings(m_fontManager, parent);
    m_glyphManager = new GlyphManager(m_appSettings, parent);
}

AppContext::~AppContext()
{

}
