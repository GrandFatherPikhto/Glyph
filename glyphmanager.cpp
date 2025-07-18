#include "glyphmanager.h"
#include "freetyperender.h"
#include "userglyphrender.h"

GlyphManager::GlyphManager(QObject *parent)
    : QObject{parent}
    , m_ftRender(QSharedPointer<IGlyphRender>())
    , m_temporaryGlyphKey(QSharedPointer<GlyphKey>())
    , m_temporaryGlyphMeta(QSharedPointer<GlyphMeta>())
    , m_temporaryTemplateLayer(QSharedPointer<QImage>())
    , m_temporaryPreviewLayer(QSharedPointer<QImage>())
    , m_temporaryUserLayer(QSharedPointer<QImage>())
{
    m_ftRender = QSharedPointer<FreeTypeRender>::create();
    m_userGlyphRender = QSharedPointer<UserGlyphRender>::create();

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

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, const QFont &font, const QString &fontPath)
{
    if (character == QChar() || bitmapDimension < 6 || glyphSize < 6)
    {
        // qDebug() << __FILE__ << __LINE__ << character << bitmapDimension;
        return QSharedPointer<GlyphMeta>();
    }


    GlyphKey key(character, bitmapDimension, font);
    if (!m_temporaryGlyphMeta.isNull() && m_temporaryGlyphMeta->glyphKey() == key)
    {
        m_temporaryGlyphMeta = getTemporaryGlyphMeta(character, bitmapDimension, 14, font, fontPath);
        qDebug() << QString("%1:%2 %3 %4").arg(__FILE__, QString::number(__LINE__), __FUNCTION__, m_temporaryGlyphMeta->toString());
        return m_temporaryGlyphMeta;
    }

    QSharedPointer<GlyphMeta> glyphMeta = find(key);

    if(!glyphMeta.isNull())
        return glyphMeta;

    glyphMeta = QSharedPointer<GlyphMeta>::create(
        character,
        bitmapDimension,
        glyphSize,
        font,
        fontPath
        );

    qDebug() << QString("%1:%2 %3 %4").arg(__FILE__, QString::number(__LINE__), __FUNCTION__, glyphMeta->toString());
    m_metaGlyphs.append(glyphMeta);
    m_index.insert(key, m_metaGlyphs.indexOf(glyphMeta));

    updateData();

    return glyphMeta;
}

QSharedPointer<QImage> GlyphManager::getTemplateGlyph(const GlyphKey &key, const QColor &color, QSharedPointer<IGlyphRender> userRenderer)
{
    if (!m_temporaryGlyphKey.isNull() && *m_temporaryGlyphKey.data() == key)
    {
        // qDebug() << "Temporary Glyph Template: " << m_temporaryGlyphMeta->toString();
// qDebug() << QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return getTemporaryTemplateLayer(color, userRenderer);
    }

    auto glyphMeta = find(key);

// qDebug() << QString("%1:%2 %3 %4").arg(__FILE__, QString::number(__LINE__), __FUNCTION__,glyphMeta->toString());

    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return QSharedPointer<QImage>();

    auto it = m_templateGlyphs.find(key);
    if (!glyphMeta->dirty() && it != m_templateGlyphs.end())
    {
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_ftRender;
    QSharedPointer<QImage> img =
        renderer->renderGlyph(glyphMeta,
            QSize(glyphMeta->glyphSize(), glyphMeta->glyphSize()),
            color);
    glyphMeta->setGlyphRect(renderer->renderRect());
    if (it != m_templateGlyphs.end())
    {
        m_templateGlyphs.erase(it);
    }

    m_templateGlyphs.insert(key, img);

    return img;
}

QSharedPointer<QImage> GlyphManager::getUserGlyph(const GlyphKey &key, const QColor &color, QSharedPointer<IGlyphRender> userRenderer)
{
    if (!m_temporaryGlyphKey.isNull() && *m_temporaryGlyphKey.data() == key)
    {
        return getTemporaryUserLayer(color, userRenderer);
    }

    // qDebug() << "Request User Glyph";
    auto glyphMeta = find(key);
    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return QSharedPointer<QImage>();

    // qDebug() << "Find user glyph in hash " << key.toString();
    auto it = m_userGlyphs.find(key);
    if (it != m_userGlyphs.end())
    {
        // qDebug() << "User glyph also rendered. Return";
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_userGlyphRender;
    QSharedPointer<QImage> img = 
        renderer->renderGlyph(glyphMeta, 
            QSize(glyphMeta->bitmapDimension(), 
            glyphMeta->bitmapDimension()), color);

    if (it != m_userGlyphs.end())
    {
        m_userGlyphs.erase(it);
    }

    m_userGlyphs.insert(key, img);

    return img;
}

QSharedPointer<QImage> GlyphManager::getPreviewGlyph(const GlyphKey &key, const QSize &previewSize, const QColor &color,  QSharedPointer<IGlyphRender> userRenderer)
{
    auto glyphMeta = find(key);
    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return QSharedPointer<QImage>();

    auto it = m_previewGlyphs.find(key);
    if (!(glyphMeta->dirty() || it == m_previewGlyphs.end() || glyphMeta->resized()))
    {
        return it.value();
    }

    if (it == m_previewGlyphs.end() && !m_temporaryGlyphKey.isNull() && *m_temporaryGlyphKey.data() == key)
    {
        return getTemporaryPreviewLayer(previewSize, color, userRenderer);
    }


    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_ftRender;
    QSharedPointer<QImage> img = renderer->renderGlyph(glyphMeta, previewSize, color);
    glyphMeta->setPreviewRect(renderer->renderRect());

    if (it != m_previewGlyphs.end())
    {
        m_previewGlyphs.erase(it);
    }

    m_previewGlyphs.insert(key, img);

    return img;
}

void GlyphManager::sortGlyphs() {
    // 1. Сортируем вектор умных указателей
    std::sort(m_metaGlyphs.begin(), m_metaGlyphs.end(),
              [](const QSharedPointer<GlyphMeta>& a, const QSharedPointer<GlyphMeta>& b) {
                  return a->character().unicode() < b->character().unicode();
              });

    // 2. Перестраиваем хеш
    m_index.clear();
    for (int i = 0; i < m_metaGlyphs.size(); ++i) {
        const auto& metaGlyph = m_metaGlyphs.at(i);
        m_index.insert(metaGlyph->glyphKey(), i);  // Используем -> вместо .
    }
}

void GlyphManager::updateData()
{
    sortGlyphs();
}


QSharedPointer<GlyphMeta> GlyphManager::getTemporaryGlyphMeta(const QChar &character, int bitmapDimension, int glyphSize, const QFont &font, const QString &fontPath)
{
    if (!m_temporaryGlyphMeta.isNull())
    {
        m_temporaryGlyphMeta.clear();
    }

    m_temporaryGlyphMeta = QSharedPointer<GlyphMeta>::create(character, bitmapDimension, glyphSize, font, fontPath);
    m_temporaryGlyphKey = QSharedPointer<GlyphKey>::create(m_temporaryGlyphMeta->glyphKey());
    m_temporaryGlyphMeta->setDirty();

    qDebug() << QString("%1:%2 %3 %4").arg(__FILE__, QString::number(__LINE__), __FUNCTION__,m_temporaryGlyphMeta->toString());

    return m_temporaryGlyphMeta;
}

QSharedPointer<QImage> GlyphManager::getTemporaryTemplateLayer(const QColor &color, QSharedPointer<IGlyphRender> userRenderer)
{
    if (m_temporaryGlyphMeta.isNull())
    {
        return QSharedPointer<QImage>();
    }

    if (!m_temporaryTemplateLayer.isNull())
    {
        if (m_temporaryGlyphMeta->dirty())
        {
            m_temporaryTemplateLayer.clear();
        } else
        {
            return m_temporaryTemplateLayer;
        }
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
    m_temporaryTemplateLayer =
        renderer->renderGlyph(m_temporaryGlyphMeta,
            QSize(m_temporaryGlyphMeta->glyphSize(), m_temporaryGlyphMeta->glyphSize()),
            color);
    m_temporaryGlyphMeta->setGlyphRect(renderer->renderRect());
// qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_temporaryGlyphMeta->toString() << ", Width: " << m_temporaryTemplateLayer->width() << ", Height: " << m_temporaryTemplateLayer->height();

    return m_temporaryTemplateLayer;
}

QSharedPointer<QImage> GlyphManager::getTemporaryPreviewLayer(const QSize &previewSize, const QColor &color, QSharedPointer<IGlyphRender> userRenderer)
{
    if (m_temporaryGlyphMeta.isNull())
        return QSharedPointer<QImage>();

    if (m_temporaryPreviewLayer.isNull() || m_temporaryPreviewLayer->size() != previewSize)
    {
        if (!m_temporaryPreviewLayer.isNull())
        {
            if (m_temporaryGlyphMeta->resized() || m_temporaryGlyphMeta->dirty())
            {
                m_temporaryPreviewLayer.clear();
            } else
            {
                return m_temporaryPreviewLayer;
            }
        }

        QSharedPointer<IGlyphRender> renderer = userRenderer.isNull() ? m_ftRender : userRenderer;
        m_temporaryPreviewLayer = renderer->renderGlyph(m_temporaryGlyphMeta, previewSize, color);
        m_temporaryGlyphMeta->setPreviewRect(renderer->renderRect());
    }

    return m_temporaryPreviewLayer;
}

QSharedPointer<QImage> GlyphManager::getTemporaryUserLayer(const QColor &color, QSharedPointer<IGlyphRender> userRenderer)
{
    if (m_temporaryGlyphMeta.isNull())
        return QSharedPointer<QImage>();

    if (!m_temporaryUserLayer.isNull())
    {
        if (m_temporaryGlyphMeta->dirty())
        {
            m_temporaryUserLayer.clear();
        } else
        {
            return m_temporaryUserLayer;
        }
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_userGlyphRender;
    m_temporaryUserLayer =
        renderer->renderGlyph(m_temporaryGlyphMeta,
            QSize(m_temporaryGlyphMeta->bitmapDimension(),
            m_temporaryGlyphMeta->bitmapDimension()), color);


    return m_temporaryUserLayer;
}
