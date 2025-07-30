#include "glyphmanager.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"

GlyphManager::GlyphManager(FontManager *fontManager, AppSettings *appSettings, QObject *parent)
    : QObject{parent}
    , m_appSettings(appSettings)
    , m_fontManager(fontManager)
    , m_glyph(QSharedPointer<GlyphContext>())
    , m_bitmapDimensions(nullptr)
{
    m_bitmapDimensions = new BitmapDimensions(this);
    setupSignals();
}

GlyphManager::~GlyphManager()
{

}

void GlyphManager::resetHash ()
{
    m_glyphs.clear();
    m_index.clear ();
}

void GlyphManager::setupSignals ()
{

}

QSharedPointer<GlyphContext> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, bool temporary)
{
    if (character == QChar() || bitmapDimension < 6 || glyphSize < 6)
    {
        return m_glyph;
    }

    GlyphKey key = GlyphKey(character, bitmapDimension);

    if (!m_glyph.isNull() && m_glyph->key() == key)
    {
        return m_glyph;
    }

    m_glyph = find(key);

    if (m_glyph.isNull())
    {
        QSharedPointer<BitmapDimension> dimension = m_bitmapDimensions->registerBitmapDimension(bitmapDimension);
        QSharedPointer<GlyphEntry> entry = QSharedPointer<GlyphEntry>::create(dimension, character, glyphSize, m_fontManager->glyphFont(), m_fontManager->glyphFontPath());
        m_glyph = QSharedPointer<GlyphContext>::create(dimension, entry);
        append(m_glyph);
    }

    return m_glyph;
}

QSharedPointer<GlyphContext> GlyphManager::find(const GlyphKey &key)
{
    QChar ch(key.character());
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_glyphs.at(it.value());
    }

    return QSharedPointer<GlyphContext>();
}


bool GlyphManager::remove(const GlyphKey &key)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_index.find(key);
    if (it == m_index.end())
        return false;

    // Получаем метаданные глифа перед удалением
    if (it.value() >= 0 && it.value() < m_glyphs.size())
    {
        auto glyphMeta = m_glyphs.at(it.value());
        m_bitmapDimensions->releaseBitmapDimension(key.bitmapDimension());
    }

    m_glyphs.removeAt(it.value());
    m_index.remove(key);
    
    updateHash();

    return true;
}

bool GlyphManager::append(QSharedPointer<GlyphContext> glyph)
{
    if (glyph.isNull())
        return false;

    QMutexLocker locker(&m_mutex);

    auto key = glyph->key();
    
    if (m_index.contains(key))
        return false;

    m_glyphs.append(glyph);
    m_index.insert(key, m_glyphs.size() - 1);

    updateHash();
 
    return true;
}

void GlyphManager::sortHash ()
{
    // 1. Сортируем вектор умных указателей
    std::sort(m_glyphs.begin(), m_glyphs.end(),
              [](const QSharedPointer<GlyphContext>& a, const QSharedPointer<GlyphContext>& b) {
                  return a->glyphEntry()->character().unicode() < b->glyphEntry()->character().unicode();
              });

    // 2. Перестраиваем хеш
    m_index.clear();
    for (int i = 0; i < m_glyphs.size(); ++i) {
        const auto& glyph = m_glyphs.at(i);
        m_index.insert(glyph->key(), i);
    }
}

void GlyphManager::updateHash ()
{
    sortHash();
    emit glyphsHashChanged ();
}
