#include "glyphmanager.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"

GlyphManager::GlyphManager(FontManager *fontManager, AppSettings *appSettings, QObject *parent)
    : QObject{parent}
    , m_appSettings(appSettings)
    , m_fontManager(fontManager)
    , m_ftRender(QSharedPointer<IGlyphRenderer>())
    // , m_glyphKey(QSharedPointer<GlyphKey>())
    , m_glyphMeta(QSharedPointer<GlyphMeta>())
    , m_bitmapDimensions(nullptr)
{
    m_bitmapDimensions = new BitmapDimensions(this);
    m_ftRender = QSharedPointer<FreeTypeGlyphRenderer>::create();
    m_drawRender = QSharedPointer<DrawGlyphRenderer>::create();

    m_index.clear();
    m_metaGlyphs.clear();

    setupSignals();
}

GlyphManager::~GlyphManager()
{

}

void GlyphManager::setupSignals()
{
    QObject::connect(this, &GlyphManager::clearDrawLayer, this, [=](){ clearDrawLayer(); });
    QObject::connect(this, &GlyphManager::pasteTemplateToDrawLayer, this, [=](){ execPasteTemplateToDrawLayer (); });
    QObject::connect(this, &GlyphManager::renderGlyphLayers, this, &GlyphManager::execRenderGlyphLayers);

    QObject::connect(this, &GlyphManager::glyphOffsetLeft, this, &GlyphManager::setGlyphOffsetMoveLeft);
    QObject::connect(this, &GlyphManager::glyphOffsetTop, this, &GlyphManager::setGlyphOffsetMoveTop);
    QObject::connect(this, &GlyphManager::glyphOffsetRight, this, &GlyphManager::setGlyphOffsetMoveRight);
    QObject::connect(this, &GlyphManager::glyphOffsetDown, this, &GlyphManager::setGlyphOffsetMoveDown);
    QObject::connect(this, &GlyphManager::glyphOffsetReset, this, &GlyphManager::setGlyphOffsetReset);
}

QSharedPointer<GlyphMeta> GlyphManager::execClearDrawLayer(const QColor &bgColor)
{
    if (m_glyphMeta.isNull() || m_glyphMeta->layerDraw().isNull())
        return QSharedPointer<GlyphMeta>();

    m_glyphMeta->layerDraw()->fill(bgColor == QColor() ? m_appSettings->drawBgColor() : bgColor);

    return m_glyphMeta;
}

QSharedPointer<GlyphMeta> GlyphManager::execPasteTemplateToDrawLayer(const QColor &color, const QColor &bgColor)
{
    Q_UNUSED(color)
    Q_UNUSED(bgColor)

    if (m_glyphMeta.isNull() || m_glyphMeta->layerTemplate().isNull() || m_glyphMeta->layerDraw().isNull())
        return QSharedPointer<GlyphMeta>();

    QImage srcImg = QImage(*(m_glyphMeta->layerTemplate().data()));
    QRect glyphRect = m_glyphMeta->paintRect();
    for (int x = 0; x < m_glyphMeta->layerTemplate()->width(); x++)
    {
        for (int y = 0; y < m_glyphMeta->layerTemplate()->height(); y++)
        {
            if (srcImg.pixelColor(x, y).alpha())
            {
                srcImg.setPixelColor(x, y, QColor(0x0, 0x0, 0x0, 0xFF));
            }
        }
    }
    QPainter painter(m_glyphMeta->layerDraw().data());
    painter.drawImage(glyphRect, srcImg);
    painter.end();

    emit layerDrawChanged(m_glyphMeta);

    return m_glyphMeta;
}

bool GlyphManager::remove(const GlyphKey &key)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_index.find(key);
    if (it == m_index.end())
        return false;

    // Получаем метаданные глифа перед удалением
    if (it.value() >= 0 && it.value() < m_metaGlyphs.size())
    {
        auto glyphMeta = m_metaGlyphs[it.value()];
        m_bitmapDimensions->releaseBitmapDimension(key.bitmapDimension());
    }

    m_metaGlyphs.removeAt(it.value());
    m_index.remove(key);
    updateData();

    return true;
}

bool GlyphManager::append(QSharedPointer<GlyphMeta> glyphMeta)
{
    QMutexLocker locker(&m_mutex);
    if (!glyphMeta)
        return false;

    auto key = glyphMeta->key();
    if (m_index.contains(key))
        return false;

    m_metaGlyphs.append(glyphMeta);
    m_index.insert(key, m_metaGlyphs.size() - 1);
    m_bitmapDimensions->registerBitmapDimension(glyphMeta->bitmapDimension());

    updateData();
 
    return true;
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

const GlyphKey GlyphManager::currentGlyphParams(const QChar &character, int bitmapDimension, int glyphSize)
{
    GlyphKey key (character, bitmapDimension);
    if (!m_glyphMeta.isNull() && key == m_glyphMeta->key())
    {
        if (m_glyphMeta->glyphSize() != glyphSize)
        {
            m_glyphMeta->setGlyphSize(glyphSize);
            m_glyphMeta->setDirty();
        }

        if (m_glyphMeta->font().family() != m_fontManager->glyphFont().family())
        {
            m_glyphMeta->setFont(m_fontManager->glyphFont());
            m_glyphMeta->setFontPath(m_fontManager->glyphFontPath());
            m_glyphMeta->setDirty();
        }

        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Temporary!";
    }
    return key;
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, bool temporary)
{
    if (character == QChar() || bitmapDimension < 6 || glyphSize < 6)
    {
        return QSharedPointer<GlyphMeta>();
    }

    GlyphKey key = currentGlyphParams(character, bitmapDimension, glyphSize);
    m_glyphMeta = find(key);

    if (m_glyphMeta.isNull())
    {
        m_glyphMeta = QSharedPointer<GlyphMeta>::create(
            character,
            bitmapDimension,
            glyphSize,
            m_fontManager->glyphFont(),
            m_fontManager->glyphFontPath(),
            temporary,
            true, // Dirty
            true // Resized
            );
    } else if (temporary)
    {
        remove(key);
    }

    if (!temporary && append(m_glyphMeta))
    {
        updateData();
    }

    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << ", Size: " << m_metaGlyphs.size();

    return m_glyphMeta;
}

void GlyphManager::setBitmapDimension(int value)
{
    if (m_bitmapDimension != value)
    {
        m_bitmapDimension = value;
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreate(m_character, m_bitmapDimension, m_glyphSize);
        if (glyphMeta.isNull())
        {
            return;
        }
        filterGlyphs ();
    }
}

void GlyphManager::setGlyphSize(int value)
{
    if (m_glyphSize != value)
    {
        findOrCreate(m_character, m_bitmapDimension, m_glyphSize);
    }
}

void GlyphManager::renderTemplateImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRenderer> userRenderer)
{
    if (glyphMeta.isNull())
        return;

    if (glyphMeta->isDirty() || glyphMeta->layerTemplate().isNull())
    {
        QSharedPointer<IGlyphRenderer>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
        glyphMeta->setLayerTemplate(renderer->renderGlyph(glyphMeta, color, bgColor, size));
        glyphMeta->setTemplateRect(renderer->renderRect());
    }
}

void GlyphManager::renderPreviewImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &previewSize, QSharedPointer<IGlyphRenderer> userRenderer)
{
    if (glyphMeta.isNull())
        return;

    QSharedPointer<IGlyphRenderer>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;

    renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
    glyphMeta->setLayerPreview(renderer->renderGlyph(glyphMeta, color, bgColor, previewSize));
    glyphMeta->setPreviewRect(renderer->renderRect());
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << glyphMeta->previewRect() << glyphMeta->layerPreview()->rect();
}

void GlyphManager::renderDrawImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRenderer> userRenderer)
{
    QSharedPointer<IGlyphRenderer>renderer = userRenderer.isNull() ? m_ftRender : userRenderer;

    renderer = userRenderer.isNull() ? m_drawRender : userRenderer;
    QSize targetSize = (size == QSize()) ? QSize(glyphMeta->bitmapDimension(), glyphMeta->bitmapDimension()) : size;
    glyphMeta->setLayerDraw(renderer->renderGlyph(glyphMeta, color, bgColor, targetSize));
    glyphMeta->setDrawRect(renderer->renderRect());
}

void GlyphManager::updateData()
{
    sort();
    emit glyphDataChanged ();
}

QSharedPointer<IGlyphRenderer> GlyphManager::getRenderer (GlyphManager::ImageType type)
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

    return QSharedPointer<IGlyphRenderer>();
}

void GlyphManager::execRenderGlyphLayers(const QSize &size)
{
    if (m_glyphMeta.isNull())
        return;

    if (m_glyphMeta->isDirty())
    {
        renderTemplateImage(m_glyphMeta, m_appSettings->templateColor(), m_appSettings->templateBgColor());
        renderDrawImage(m_glyphMeta, m_appSettings->drawColor(), m_appSettings->drawBgColor());
        emit glyphTemplateLayerRendered(m_glyphMeta);
        emit glyphDrawLayerRendered(m_glyphMeta);
    }

    if (m_glyphMeta->isResized())
    {
        renderPreviewImage(m_glyphMeta, m_appSettings->previewColor(), m_appSettings->previewBgColor(), size);
        emit glyphPreviewLayerRendered(m_glyphMeta);
    }
}

void GlyphManager::filterGlyphs()
{

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

void GlyphManager::setGlyphOffsetMoveLeft()
{
    QSharedPointer<GlyphMeta> glyphMeta = findOrCreate();
    if (glyphMeta.isNull())
        return;
    glyphMeta->moveLeft();
    emit glyphChanged(glyphMeta);
}

void GlyphManager::setGlyphOffsetMoveTop()
{
    QSharedPointer<GlyphMeta> glyphMeta = findOrCreate();
    if (glyphMeta.isNull())
        return;
    glyphMeta->moveTop();
    emit glyphChanged(glyphMeta);
}

void GlyphManager::setGlyphOffsetMoveDown()
{
    QSharedPointer<GlyphMeta> glyphMeta = findOrCreate();
    if (glyphMeta.isNull())
        return;
    glyphMeta->moveDown();
    emit glyphChanged(glyphMeta);
}

void GlyphManager::setGlyphOffsetMoveRight()
{
    QSharedPointer<GlyphMeta> glyphMeta = findOrCreate();
    if (glyphMeta.isNull())
        return;
    glyphMeta->moveRight();
    emit glyphChanged(glyphMeta);
}

void GlyphManager::setGlyphOffsetReset()
{
    QSharedPointer<GlyphMeta> glyphMeta = findOrCreate();
    if (glyphMeta.isNull())
        return;
    glyphMeta->resetOffset();
    emit glyphChanged(glyphMeta);
}
