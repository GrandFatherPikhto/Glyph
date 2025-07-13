#include "glyphmanager.h"

GlyphManager::GlyphManager(QObject *parent)
    : QObject{parent}
{
    m_index.clear();
    m_glyphs.clear();
}

QSharedPointer<Glyph> GlyphManager::findOrCreate(const QChar &character, int gridSize)
{
    GlyphKey key(character, gridSize);
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_glyphs[it.value()];
    }

    // Glyph glyph(font, character, gridSize);
    auto glyph = QSharedPointer<Glyph>::create(
        character,
        gridSize
        );
    m_glyphs.append(glyph);
    m_index.insert(key, m_glyphs.indexOf(glyph));

    sortGlyphs();

    return glyph;
}

void GlyphManager::sortGlyphs() {
    // 1. Сортируем вектор умных указателей
    std::sort(m_glyphs.begin(), m_glyphs.end(),
              [](const QSharedPointer<Glyph>& a, const QSharedPointer<Glyph>& b) {
                  return a->character().unicode() < b->character().unicode();
              });

    // 2. Перестраиваем хеш
    m_index.clear();
    for (int i = 0; i < m_glyphs.size(); ++i) {
        const auto& glyph = m_glyphs.at(i);
        m_index.insert(glyph->glyphKey(), i);  // Используем -> вместо .
    }
}
