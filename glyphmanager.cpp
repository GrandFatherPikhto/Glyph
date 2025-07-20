#include "glyphmanager.h"
#include "freetyperender.h"
#include "drawglyphrender.h"

GlyphManager::GlyphManager(AppSettings *applicationSettings, QObject *parent)
    : QObject{parent}
    , m_applicationSettings(applicationSettings)
    , m_ftRender(QSharedPointer<IGlyphRender>())
    , m_glyphKey(QSharedPointer<GlyphKey>())
    , m_glyphMeta(QSharedPointer<GlyphMeta>())
{
    m_ftRender = QSharedPointer<FreeTypeRender>::create();
    m_drawRender = QSharedPointer<DrawGlyphRender>::create();

    m_index.clear();
    m_metaGlyphs.clear();
}

GlyphManager::~GlyphManager()
{
}

QSharedPointer<GlyphMeta> GlyphManager::find(const GlyphKey &key)
{
    QChar ch(key.character());
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_metaGlyphs[it.value()];
    }

    return QSharedPointer<GlyphMeta>();
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, const QFont &font, const QString &fontPath, bool temporary)
{
    if (character == QChar() || bitmapDimension < 6 || glyphSize < 6)
    {
        return QSharedPointer<GlyphMeta>();
    }

    GlyphKey key (character, bitmapDimension, font);
    QSharedPointer<GlyphMeta> glyphMeta = find(key);

    if (glyphMeta.isNull() || (!m_glyphMeta.isNull() && m_glyphMeta->key() != glyphMeta->key()))
    {
        glyphMeta = QSharedPointer<GlyphMeta>::create(
            character,
            bitmapDimension,
            glyphSize,
            font,
            fontPath,
            temporary,
            true
        );
    }

    if(!temporary)
    {
        m_metaGlyphs.append(glyphMeta);
        m_index.insert(key, m_metaGlyphs.indexOf(glyphMeta));
        updateData();
    }

    m_glyphMeta = glyphMeta;
    
    return glyphMeta;
}

void GlyphManager::renderGlyphLayers(QSharedPointer<GlyphMeta> glyphMeta, const QSize &previewSize)
{
    renderTemplateImage(glyphMeta, m_applicationSettings->templateColor(), m_applicationSettings->templateBgColor());
    renderPreviewImage(glyphMeta, m_applicationSettings->previewColor(), m_applicationSettings->previewBgColor(), previewSize);
    renderDrawImage(glyphMeta, m_applicationSettings->drawColor(), m_applicationSettings->drawBgColor());
}

void GlyphManager::renderTemplateImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRender> userRenderer)
{
    if (glyphMeta.isNull())
        return;

    if (glyphMeta->isDirty() || glyphMeta->layerTemplate().isNull())
    {
        QSharedPointer<IGlyphRender>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
        QSize targetSize = (size == QSize()) ?  QSize(glyphMeta->glyphSize(), glyphMeta->glyphSize()) : size;
        glyphMeta->setLayerTemplate(renderer->renderGlyph(glyphMeta, color, bgColor, targetSize));
        glyphMeta->setTemplateRect(renderer->renderRect());
    }
}

void GlyphManager::renderPreviewImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRender> userRenderer)
{
    if (glyphMeta.isNull())
        return;

    QSharedPointer<IGlyphRender>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;

    renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
    glyphMeta->setLayerPreview(renderer->renderGlyph(glyphMeta, color, bgColor, size));
    glyphMeta->setPreviewRect(renderer->renderRect());
}

void GlyphManager::renderDrawImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRender> userRenderer)
{
    QSharedPointer<IGlyphRender>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;

    renderer = userRenderer.isNull() ? m_drawRender : userRenderer;
    QSize targetSize = (size == QSize()) ? QSize(glyphMeta->bitmapDimension(), glyphMeta->bitmapDimension()) : size;
    glyphMeta->setLayerPreview(renderer->renderGlyph(glyphMeta, color, bgColor, targetSize));
    glyphMeta->setDrawRect(renderer->renderRect());
}

void GlyphManager::updateData()
{
    sort();
}

QSharedPointer<IGlyphRender> GlyphManager::getRenderer (GlyphManager::ImageType type)
{
    switch (type)
    {
    case GlyphManager::ImageTemplate :
        return m_ftRender;
    
    case GlyphManager::ImagePreview :
        return m_ftRender;

    case GlyphManager::ImageDraw :
        return m_drawRender;
    default:
        break;
    }

    return QSharedPointer<IGlyphRender>();
}

void GlyphManager::sort() {
    // 1. Сортируем вектор умных указателей
    std::sort(m_metaGlyphs.begin(), m_metaGlyphs.end(),
              [](const QSharedPointer<GlyphMeta>& a, const QSharedPointer<GlyphMeta>& b) {
                  return a->character().unicode() < b->character().unicode();
              });

    // 2. Перестраиваем хеш
    m_index.clear();
    for (int i = 0; i < m_metaGlyphs.size(); ++i) {
        const auto& metaGlyph = m_metaGlyphs.at(i);
        m_index.insert(metaGlyph->key(), i);  // Используем -> вместо .
    }
}
