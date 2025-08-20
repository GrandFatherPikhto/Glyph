#include "appcontext.h"
#include "appsettings.h"
#include "profilemanager.h"
#include "glyphdrawcontext.h"
#include "glyphmanager.h"
#include "fontmanager.h"
#include "imagemanager.h"

GlyphDrawContext::GlyphDrawContext(AppContext *appContext, QObject *parent)
    : QObject{parent}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_profileManager(appContext->profileManager())
    , m_glyphManager(appContext->glyphManager())
    , m_imageManager(appContext->imageManager())
    , m_fontManager(appContext->fontManager())
{
    setupSignals();
    setProfile(m_profileManager->profile());
    setGlyph(m_glyphManager->glyph());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

GlyphDrawContext::~GlyphDrawContext()
{

}

void GlyphDrawContext::setupValues()
{
    setProfile(m_profileManager->profile());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

void GlyphDrawContext::setupSignals()
{
    connect(m_profileManager, &ProfileManager::changeProfile, this, &GlyphDrawContext::setProfile);
    connect(m_glyphManager, &GlyphManager::changeGlyph, this, &GlyphDrawContext::setGlyph);
    connect(m_appSettings, &AppSettings::glyphWidgetMarginsChanged, this, &GlyphDrawContext::setMargins);
}

void GlyphDrawContext::setGlyph(const GlyphContext &glyph)
{
    m_glyph = glyph;
    renderTemplate();
    renderPreview();
    renderDraw();
}

void GlyphDrawContext::setProfile(const ProfileContext &profile)
{
    m_profile = profile;
}

void GlyphDrawContext::setMargins(const QMargins &margins)
{
    m_margins = margins;
    m_drawRect -= m_margins;
}

void GlyphDrawContext::setRegion(const QRegion &region)
{
    if (region.isEmpty())
        return;
    m_region = region;

    m_drawRect = m_region.boundingRect();
    m_drawRect -= m_margins;
}

bool GlyphDrawContext::renderTemplate()
{
    if (!m_glyph.isValid())
        return false;

    FontContext font = m_fontManager->fontContext();

    if (m_template.isNull())
    {
        m_template = QSharedPointer<ImageContext>::create();
    }

    m_template->setCharacter(m_glyph.character());
    m_template->setDirty();
    m_template->setSize(QSize(m_glyph.size(), m_glyph.size()));
    m_template->setType(ImageContext::ImageTemplate);
    m_template->setColor(m_appSettings->value("glyphWidgetTemplateColor").value<QColor>());
    m_template->setBgColor(m_appSettings->value("glyphWidgetTemplateBgColor").value<QColor>());
    m_imageManager->getGlyphImage(font, m_template);
    qDebug() << __FILE__ << __LINE__ << "Template:" << m_template;

    QSize glyphSize(m_template->width() * cellSize(),
                    m_template->height() * cellSize());

    ProfileContext profile = m_profileManager->glyphProfile(m_glyph);

    int bottom = profile.bitmapDimension() - m_template->top();
    m_glyphRect = QRect(glyphTopLeft() + QPoint(m_template->left() * cellSize(), bottom * cellSize()), glyphSize);

    return true;
}

bool GlyphDrawContext::renderPreview()
{
    if (!m_glyph.isValid())
        return false;

    FontContext font = m_fontManager->fontContext();

    if (m_preview.isNull())
    {
        m_preview = QSharedPointer<ImageContext>::create();
    }

    if (bitmapRect().size().isValid())
        return false;

    m_preview->setCharacter(m_glyph.character());
    m_preview->setDirty();
    m_preview->setSize(bitmapRect().size());
    m_preview->setType(ImageContext::ImageTemplate);
    m_preview->setColor(m_appSettings->value("glyphWidgetPreviewColor").value<QColor>());
    m_preview->setBgColor(m_appSettings->value("glyphWidgetPreviewBgColor").value<QColor>());
    m_imageManager->getGlyphImage(font, m_preview);
    qDebug() << __FILE__ << __LINE__ << "Preview:" << m_preview;

    return true;
}

bool GlyphDrawContext::renderDraw()
{
    if (!m_glyph.isValid())
        return false;

    ProfileContext profile;

    if (m_draw.isNull())
    {
        m_draw = QSharedPointer<DrawContext>::create();
    }

    profile = m_profileManager->glyphProfile(m_glyph);
    if (!profile.isValid())
        return false;

    m_draw->setSize(QSize(profile.bitmapDimension(), profile.bitmapDimension()));
    m_draw->setGlyphId(m_glyph.id());

    m_imageManager->loadOrCreateDrawImage(m_glyph.id(), m_draw);

    return true;    
}
