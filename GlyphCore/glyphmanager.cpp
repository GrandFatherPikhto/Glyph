#include "glyphmanager.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"


GlyphManager::GlyphManager(FontManager *fontManager, QObject *parent)
    : QObject{parent}
    , m_fontManager(fontManager)
    , m_ftRender(QSharedPointer<IGlyphRenderer>())
    , m_glyphKey(QSharedPointer<GlyphKey>())
    , m_glyphMeta(QSharedPointer<GlyphMeta>())
{
    m_ftRender = QSharedPointer<FreeTypeGlyphRenderer>::create();
    m_drawRender = QSharedPointer<DrawGlyphRenderer>::create();

    m_index.clear();
    m_metaGlyphs.clear();
}

GlyphManager::~GlyphManager()
{

}

void GlyphManager::generateBitmapDimensionValues()
{
    m_bitmapDimensionValues = m_bitmapDimensions.keys();
    std::sort(m_bitmapDimensionValues.begin(), m_bitmapDimensionValues.end());
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_bitmapDimensions.size();

    emit bitmapDimensionsChanged();
}

QSharedPointer<BitmapDimension> GlyphManager::bitmapDimension(int value)
{
    auto it = m_bitmapDimensions.find(value);
    if (it == m_bitmapDimensions.end())
    {
        return QSharedPointer<BitmapDimension>();
    }

    return it.value();
}

QSharedPointer<BitmapDimension> GlyphManager::bitmapDimensionAt(int pos)
{
    if (pos < 0 || pos >= m_bitmapDimensionValues.size())
        return QSharedPointer<BitmapDimension>();
    
    int bitmapDimension = m_bitmapDimensionValues.at(pos);
    auto it = m_bitmapDimensions.find(bitmapDimension);

    if (it == m_bitmapDimensions.end())
    {
        return QSharedPointer<BitmapDimension>();
    }

    return it.value();
}

bool GlyphManager::appendBitmapDimension(int bitmapDimension)
{
    auto it = m_bitmapDimensions.find(bitmapDimension);
    if (it == m_bitmapDimensions.end())
    {
        // Создаем новое измерение если не найдено
        auto dimension = QSharedPointer<BitmapDimension>::create(bitmapDimension, QMargins(0,0,0,0));
        m_bitmapDimensions.insert(bitmapDimension, dimension);
        generateBitmapDimensionValues();

        return true;
    }

    // Увеличиваем счетчик существующего измерения
    it.value()->incrementCounter();

    return false;
}


bool GlyphManager::removeBitmapDimension(int value)
{
    auto it = m_bitmapDimensions.find(value);
    if (it == m_bitmapDimensions.end())
        return false;

    // Декрементируем счетчик
    int newCount = it.value()->decrementCounter();
    
    // Удаляем если счетчик достиг нуля
    if (newCount <= 0)
    {
        m_bitmapDimensions.erase(it);
        generateBitmapDimensionValues();

        return true;
    }
    
    return false;
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
        removeBitmapDimension(key.bitmapDimension());
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
    appendBitmapDimension(glyphMeta->bitmapDimension());
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
    GlyphKey key (character, bitmapDimension, m_fontManager->glyphFont());
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
