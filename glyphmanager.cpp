#include "glyphmanager.h"
#include "freetyperender.h"

GlyphManager::GlyphManager(QObject *parent)
    : QObject{parent}
    , m_ftRender(nullptr)
{
    m_ftRender = QSharedPointer<FreeTypeRender>::create();
    m_index.clear();
    m_metaGlyphs.clear();
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const GlyphKey &key, const QFont font, const QString &fontPath)
{
    QChar ch(key.character());
    return findOrCreate(ch, key.gridSize(), font, fontPath);
}

QSharedPointer<GlyphMeta> GlyphManager::findOrCreate(const QChar &character, int gridSize, const QFont font, const QString &fontPath)
{
    if (character == QChar() || gridSize < 6)
        return nullptr;
        
    GlyphKey key(character, gridSize);
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_metaGlyphs[it.value()];
    }

    // Glyph glyph(font, character, gridSize);
    auto glyphMeta = QSharedPointer<GlyphMeta>::create(
        character,
        gridSize
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
    if (glyphMeta->fontPath().isEmpty())
        return nullptr;

    auto it = m_templateGlyphs.find(key);
    if (!glyphMeta->dirty() && it != m_templateGlyphs.end())
    {
        return it.value();
    }

    QSharedPointer<IGlyphRender> renderer = userRenderer ? userRenderer : m_ftRender;
    QSharedPointer<QImage> img = renderer->renderGlyph(glyphMeta);
    if (it != m_templateGlyphs.end())
    {
        m_templateGlyphs.erase(it);
    }

    m_templateGlyphs.insert(key, img);

    return img;
}

QSharedPointer<QImage> GlyphManager::getUserGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> renderer)
{
    return nullptr;
}

QSharedPointer<QImage> GlyphManager::getPreviewGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> renderer)
{
    return nullptr;
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


