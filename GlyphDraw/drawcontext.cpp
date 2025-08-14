#include "appcontext.h"
#include "appsettings.h"
#include "profilemanager.h"
#include "drawcontext.h"

DrawContext::DrawContext(AppContext *appContext, QObject *parent)
    : QObject{parent}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_profileManager(m_appContext->profileManager())
{
    setupSignals();

    setProfile(m_profileManager->profile());
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
    connect(m_appSettings, &AppSettings::glyphWidgetMarginsChanged, this, &DrawContext::setMargins);
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
