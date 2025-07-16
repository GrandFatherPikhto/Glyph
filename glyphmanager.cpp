#include "glyphmanager.h"
#include "freetyperender.h"
#include "userglyphrender.h"

GlyphManager::GlyphManager(QObject *parent)
    : QObject{parent}
    , m_ftRender(nullptr)
{
    m_ftRender = QSharedPointer<FreeTypeRender>::create();
    m_userGlyphRender = QSharedPointer<UserGlyphRender>::create();

    m_index.clear();
    m_metaGlyphs.clear();
}

GlyphManager::~GlyphManager()
{
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const GlyphKey &key, const QFont font, const QString &fontPath)
{
    QChar ch(key.character());
    return findOrCreate(ch, key.bitmapDimension(), font, fontPath);
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, const QFont font, const QString &fontPath)
{
    if (character == QChar() || bitmapDimension < 6)
    {
        qDebug() << __FILE__ << __LINE__ << character << bitmapDimension;
        return nullptr;
    }
        
    GlyphKey key(character, bitmapDimension);
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_metaGlyphs[it.value()];
    }

    // Glyph glyph(font, character, bitmapDimension);
    auto glyphMeta = QSharedPointer<GlyphMeta>::create(
        character,
        bitmapDimension
        );

    if (font != QFont())
    {
        glyphMeta->setFont(font);
    }

    if (!fontPath.isEmpty())
    {
        glyphMeta->setFontPath(fontPath);
    }

    m_metaGlyphs.append(glyphMeta);
    m_index.insert(key, m_metaGlyphs.indexOf(glyphMeta));

    sortGlyphs();

    return glyphMeta;
}

QSharedPointer<QImage> GlyphManager::getTemplateGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> userRenderer)
{
    auto glyphMeta = findOrCreate(key);

    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return nullptr;

    auto it = m_templateGlyphs.find(key);
    if (!glyphMeta->dirty() && it != m_templateGlyphs.end())
    {
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_ftRender;
    QSharedPointer<QImage> img = renderer->renderGlyph(glyphMeta, QSize(glyphMeta->glyphSize(), glyphMeta->glyphSize()), QColor(Qt::blue));
    glyphMeta->setGlyphRect(renderer->renderRect());
    if (it != m_templateGlyphs.end())
    {
        m_templateGlyphs.erase(it);
    }

    m_templateGlyphs.insert(key, img);

    return img;
}

QSharedPointer<QImage> GlyphManager::getUserGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> userRenderer)
{
    auto glyphMeta = findOrCreate(key);
    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return nullptr;

    auto it = m_userGlyphs.find(key);
    if (!(glyphMeta->dirty() || it == m_userGlyphs.end() || glyphMeta->resized()))
    {
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_userGlyphRender;
    QSharedPointer<QImage> img = renderer->renderGlyph(glyphMeta, QSize(glyphMeta->gridDimension(), glyphMeta->gridDimension()), QColor(Qt::black));

    if (it != m_userGlyphs.end())
    {
        m_userGlyphs.erase(it);
    }

    m_userGlyphs.insert(key, img);

    return img;
}

QSharedPointer<QImage> GlyphManager::getPreviewGlyph(const GlyphKey &key, const QSize &previewSize, QSharedPointer<IGlyphRender> userRenderer)
{
    auto glyphMeta = findOrCreate(key);
    if (!glyphMeta || glyphMeta->fontPath().isEmpty())
        return nullptr;

    auto it = m_previewGlyphs.find(key);
    if (!(glyphMeta->dirty() || it == m_previewGlyphs.end() || glyphMeta->resized()))
    {
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_ftRender;
    QSharedPointer<QImage> img = renderer->renderGlyph(glyphMeta, previewSize, QColor(Qt::red));
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


