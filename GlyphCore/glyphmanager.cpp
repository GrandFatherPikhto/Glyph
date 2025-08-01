#include "appcontext.h"
#include "glyphmanager.h"
#include "glyphfilter.h"
#include "appsettings.h"
#include "glyphcontext.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "bitmapdimension.h"
#include "dimensionmanager.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appSettings(nullptr)
    , m_fontManager(nullptr)
    , m_glyph(QSharedPointer<GlyphContext>())
    , m_dimensionManager(nullptr)
{
    m_glyphs = std::make_shared<QVector<QSharedPointer<GlyphContext>>>();
    setupSignals();
}

GlyphManager::~GlyphManager()
{

}

void GlyphManager::resetHash ()
{
    m_index.clear ();
}

void GlyphManager::setDimensionManager(DimensionManager * dimensionManager)
{
    m_dimensionManager = dimensionManager;
}

void GlyphManager::setFontManager(FontManager * fontManager)
{
    m_fontManager = fontManager;
}

void GlyphManager::setGlyphFilter(GlyphFilter * glyphFilter)
{
    m_glyphFilter = glyphFilter;
}

void GlyphManager::setAppSettings(AppSettings * appSettings)
{
    m_appSettings = appSettings;
}

void GlyphManager::setupSignals ()
{
    QObject::connect(this, &GlyphManager::changeCharacter, this, [=](const QChar &character, bool temporary){
        changeGlyph(character, temporary);
        // qDebug() << __FILE__ << __LINE__ << m_glyph;
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

QSharedPointer<GlyphContext> GlyphManager::findOrCreate(const QChar &character, int dimension, int size, bool temporary)
{
    int bitmapDimension = dimension <= 0 ? m_appSettings->bitmapDimension() : bitmapDimension;
    int glyphSize = size <= 0 ? m_appSettings->glyphSize() : size;

    if (character == QChar())
    {
        return m_glyph;
    }

    GlyphKey key = GlyphKey(character, bitmapDimension <= 0 ? m_appSettings->bitmapDimension() : bitmapDimension);
    // qDebug() << __FILE__ << __LINE__ << key;
    if (!m_glyph.isNull() && m_glyph->key() == key)
    {
        return m_glyph;
    }

    m_glyph = find(key);

    if (m_glyph.isNull())
    {
        QSharedPointer<BitmapDimension> dimension = m_dimensionManager->registerBitmapDimension(bitmapDimension);
        m_glyph = QSharedPointer<GlyphContext>::create(dimension, character, glyphSize, temporary, m_fontManager->glyphFont(), m_fontManager->glyphFontPath());
        append(m_glyph);
        emit glyphCreated (m_glyph);
    }

    emit glyphChanged(m_glyph);

    // qDebug() << __FILE__ << __LINE__ << m_glyph;
    
    return m_glyph;
}

QSharedPointer<GlyphContext> GlyphManager::find(const GlyphKey &key)
{
    auto it = m_index.find(key);
    if (it != m_index.end()) {
        return m_glyphs->at(it.value());
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
    if (it.value() >= 0 && it.value() < m_glyphs->size())
    {
        auto glyphContext = m_glyphs->at(it.value());
        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << *(glyphContext.data());
        m_dimensionManager->releaseBitmapDimension(key.bitmapDimension());
    }

    m_glyphs->removeAt(it.value());
    m_index.remove(key);
    
    updateHash();

    return true;
}

bool GlyphManager::append(QSharedPointer<GlyphContext> glyph)
{
    if (glyph.isNull() || m_glyphs == nullptr)
        return false;

    QMutexLocker locker(&m_mutex);

    auto key = glyph->key();
    
    if (m_index.contains(key))
        return false;

    // auto &vec = *m_glyphs;
    try {
        m_glyphs->append(glyph);
    } catch (const std::exception &e)
    {
        qCritical() << "Append failed:" << e.what();
    }

    m_index.insert(key, m_glyphs->size() - 1);

    updateHash();
 
    return true;
}

void GlyphManager::sortHash ()
{
    // 1. Сортируем вектор умных указателей
    std::sort(m_glyphs->begin(), m_glyphs->end(),
              [](const QSharedPointer<GlyphContext>& a, const QSharedPointer<GlyphContext>& b) {
                  return a->character().unicode() < b->character().unicode();
              });

    // 2. Перестраиваем хеш
    m_index.clear();
    for (int i = 0; i < m_glyphs->size(); ++i) {
        const auto& glyph = m_glyphs->at(i);
        m_index.insert(glyph->key(), i);
    }
}

void GlyphManager::updateHash ()
{
    sortHash();
    emit glyphsHashChanged ();
}

const QChar GlyphManager::character()
{
    if (m_glyph.isNull())
        return QChar();
    return m_glyph->character();
}

QSharedPointer<GlyphContext> GlyphManager::glyphAt(int pos)
{
    if (pos >= m_glyphs->size())
        return QSharedPointer<GlyphContext>();

    return m_glyphs->at(pos);
}

void GlyphManager::removeGlyphsByCharacter(const QChar &ch) {
    if (m_glyphs == nullptr || m_index.isEmpty()) return;
    
    auto& vec = *m_glyphs;
    auto it = std::remove_if(vec.begin(), vec.end(),
        [&, ch](const auto& glyph) {
            if (glyph->character() == ch) {
                m_index.remove(glyph->key());  // Удаляем из хеша
                return true;                   // Удаляем из вектора
            }
            return false;
        });
    
    vec.erase(it, vec.end());
}

bool GlyphManager::changeGlyph(const QChar &ch, bool temporary)
{
    findOrCreate(ch);
    if (m_glyph.isNull())
        return false;

    m_glyph->setTemporaryValue(temporary);

    qDebug() << __FILE__ << __LINE__ << m_glyph;

    emit glyphChanged(m_glyph);
    emit m_glyphFilter->sourceGlyphChanged(m_glyph);

    return true;
}
