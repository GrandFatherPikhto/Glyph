#include <QPainter>

#include "glyphdraw.h"

#include "appcontext.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "profilemanager.h"

#include "glyphcontext.h"

GlyphDraw::GlyphDraw(QWidget *parent)
    : QWidget{parent}
{

}

GlyphDraw::~GlyphDraw()
{

}

void GlyphDraw::setAppContext(AppContext *appContext)
{
    m_appContext = appContext;
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();
    m_profileManager = m_appContext->profileManager();

    setProfile(m_profileManager->profile());
    setGlyph(m_glyphManager->glyph());

    setupSignals();
}

void GlyphDraw::setupSignals()
{
    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, &GlyphDraw::setGlyph);
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, &GlyphDraw::setProfile);
}

void GlyphDraw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.end();
}

void GlyphDraw::setGlyph(const GlyphContext &context)
{
    m_glyph = context;
    qDebug() << __FILE__ << __LINE__ << m_glyph;
}

void GlyphDraw::setProfile(const ProfileContext &context)
{
    m_profile = context;
    qDebug() << __FILE__ << __LINE__ << m_profile;
}

void GlyphDraw::drawGrid(QPainter &painter)
{

}
