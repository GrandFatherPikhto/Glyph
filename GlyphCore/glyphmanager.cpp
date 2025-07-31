#include "appcontext.h"
#include "glyphmanager.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appSettings(appContext->appSettings())
    , m_fontManager(appContext->fontManager())
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
    QObject::connect(this, &GlyphManager::changeCharacter, this, [=](const QChar &character, bool temporary){
        findOrCreate(character, m_appSettings->bitmapDimension(), m_appSettings->glyphSize(), temporary);
    });

    QObject::connect(this, &GlyphManager::glyphOffsetReset, this, [=](){
        if (!m_glyph.isNull())
        {
            m_glyph->offsetReset();
            emit glyphChanged (m_glyph);
        }
    });

    QObject::connect(this, &GlyphManager::glyphOffsetLeft, this, [=](){
        if (!m_glyph.isNull())
        {
            m_glyph->offsetLeft();
            emit glyphChanged (m_glyph);
        }
    });

    QObject::connect(this, &GlyphManager::glyphOffsetUp, this, [=](){
        if (!m_glyph.isNull())
        {
            m_glyph->offsetUp();
            emit glyphChanged (m_glyph);
        }
    });

    QObject::connect(this, &GlyphManager::glyphOffsetRight, this, [=](){
        if (!m_glyph.isNull())
        {
            m_glyph->offsetRight();
            emit glyphChanged (m_glyph);
        }
    });

    QObject::connect(this, &GlyphManager::glyphOffsetDown, this, [=](){
        if (!m_glyph.isNull())
        {
            m_glyph->offsetDown();
            emit glyphChanged (m_glyph);
        }
    });
}

QSharedPointer<GlyphContext> GlyphManager::findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, bool temporary)
{
    QMutexLocker locker(&m_mutex);

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
        QSharedPointer<GlyphEntry> entry = QSharedPointer<GlyphEntry>::create(dimension, character, glyphSize, temporary, m_fontManager->glyphFont(), m_fontManager->glyphFontPath());
        m_glyph = QSharedPointer<GlyphContext>::create(dimension, entry);
        append(m_glyph);
        emit glyphCreated (m_glyph);
    }

    emit glyphChanged(m_glyph);
    
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
    filterGlyphs ();
    emit glyphsHashChanged ();
}

void GlyphManager::filterGlyphs ()
{
    m_filtered.clear();
    for (const auto &glyphPtr : m_glyphs)
    {
        if (m_characterFilter.isEmpty() || m_characterFilter.contains(glyphPtr->character()))
        {
            m_filtered.append(glyphPtr);
        }
    }

    std::sort(m_filtered.begin(), m_filtered.end(),
        [](const QSharedPointer<GlyphContext>& a, const QSharedPointer<GlyphContext>& b) {
            return a->glyphEntry()->character().unicode() < b->glyphEntry()->character().unicode();
        });
}

const QChar GlyphManager::character()
{
    if (m_glyph.isNull())
        return QChar();
    return m_glyph->glyphEntry()->character();
}

void GlyphManager::setCharacterFilter(const QString &filter)
{
    m_characterFilter = filter;
    filterGlyphs();
}

int GlyphManager::filteredSize ()
{
    return m_filtered.size();
}

QSharedPointer<GlyphContext> GlyphManager::glyphAt(int pos)
{
    if (pos >= m_glyphs.size())
        return QSharedPointer<GlyphContext>();

    return m_glyphs.at(pos);
}

QSharedPointer<GlyphContext> GlyphManager::filteredAt(int pos)
{
    if (pos >= m_filtered.size())
        return QSharedPointer<GlyphContext>();

    return m_filtered.at(pos);
}
