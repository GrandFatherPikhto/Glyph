#include "appcontext.h"
#include "appsettings.h"
#include "profilemanager.h"
#include "drawcontext.h"
#include "glyphmanager.h"

DrawContext::DrawContext(AppContext *appContext, QObject *parent)
    : QObject{parent}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_profileManager(m_appContext->profileManager())
    , m_glyphManager(m_appContext->glyphManager())
{
    setupSignals();
    setProfile(m_profileManager->profile());
    setGlyph(m_glyphManager->glyph());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

DrawContext::~DrawContext()
{

}

void DrawContext::setupValues()
{
    setProfile(m_profileManager->profile());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

void DrawContext::setupSignals()
{
    connect(m_profileManager, &ProfileManager::changeProfile, this, &DrawContext::setProfile);
    connect(m_glyphManager, &GlyphManager::changeGlyph, this, &DrawContext::setGlyph);
    connect(m_appSettings, &AppSettings::glyphWidgetMarginsChanged, this, &DrawContext::setMargins);
}

void DrawContext::setGlyph(const GlyphContext &glyph)
{
    m_glyph = glyph;
}

void DrawContext::setProfile(const ProfileContext &profile)
{
    m_profile = profile;
}

void DrawContext::setMargins(const QMargins &margins)
{
    m_margins = margins;
    m_drawRect -= m_margins;
}

void DrawContext::setRegion(const QRegion &region)
{
    if (region.isEmpty())
        return;
    m_region = region;

    m_drawRect = m_region.boundingRect();
    m_drawRect -= m_margins;
}
