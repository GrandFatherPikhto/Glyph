#ifndef GLYPHCONTEXT_H_
#define GLYPHCONTEXT_H_

#include <QChar>
#include <QSharedPointer>
#include <QImage>
#include <QMetaType>
#include <QVariant>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GlyphContext {
public:
    explicit GlyphContext(int glyphId = -1, const QChar &character = QChar(), int glyphSize = -1, int profile = -1, int offsetLeft = 0, int baseline = 0)
        : m_glyphId(glyphId)
        , m_character(character) 
        , m_glyphSize(glyphSize) 
        , m_profile(profile)
        , m_offsetLeft(offsetLeft)
        , m_baseline(baseline)
        {};
        
    GlyphContext(const GlyphContext &glyph)
        : m_glyphId(glyph.m_glyphId)
        , m_character(glyph.m_character)
        , m_glyphSize(glyph.m_glyphSize)
        , m_profile(glyph.m_profile)
        , m_offsetLeft(glyph.m_offsetLeft)
        , m_baseline(glyph.m_baseline)
        {}

    GlyphContext(GlyphContext *glyph)
        : m_glyphId(glyph->m_glyphId)
        , m_character(glyph->m_character)
        , m_glyphSize(glyph->m_glyphSize)
        , m_profile(glyph->m_profile)
        , m_offsetLeft(glyph->m_offsetLeft)
        , m_baseline(glyph->m_baseline)
        {}

    ~GlyphContext() {}

    const GlyphContext & operator= (const GlyphContext &glyph)
    {
        m_glyphId = glyph.m_glyphId;
        m_character = glyph.m_character;
        m_glyphSize = glyph.m_glyphSize;
        m_profile = glyph.m_profile;
        m_offsetLeft = glyph.m_offsetLeft;
        m_baseline = glyph.m_baseline;
        return *this;
    }

    bool operator== (const GlyphContext &glyph) const
    {
        return (
               m_glyphId == glyph.m_glyphId
            && m_character == glyph.m_character
            && m_glyphSize == glyph.m_glyphSize
            && m_profile == glyph.m_profile
            && m_offsetLeft == glyph.m_offsetLeft
            && m_baseline == glyph.m_baseline
        );
    }

    bool operator!= (const GlyphContext &glyph) const
    {
        return (
               m_glyphId != glyph.m_glyphId
            || m_character != glyph.m_character
            || m_glyphSize != glyph.m_glyphSize
            || m_profile != glyph.m_profile
            || m_offsetLeft == glyph.m_offsetLeft
            || m_baseline == glyph.m_baseline
        );
    }

    bool isValid() const { return (m_character != QChar() && m_glyphSize > 0 && m_profile >= 0); }
    
    int id() const { return m_glyphId; }
    const QChar & character() const { return m_character; };
    int size() const { return m_glyphSize; }
    int profile() const { return m_profile; }
    int offsetLeft() const { return m_offsetLeft; }
    int baseline() const { return m_baseline; }

    void setId(int value = -1) { m_glyphId = value; }
    void setProfileId(int value = -1) { m_profile = value; }
    void setCharacter(const QChar &value) { m_character = value; }
    void setSize(int value) { m_glyphSize = value; }
    void setOffsetLeft(int value) { m_offsetLeft = value; }
    void setBaseline(int value) { m_baseline = value; }

    operator QVariant() const {
        if(QMetaType::fromName("GlyphContext").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridOffsets не зарегистрирован
            QVariantMap map;

            map["id"] = m_glyphId;
            map["character"] = m_character;
            map["glyph_size"] = m_glyphSize;
            map["profile"] = m_profile;
            map["offset_left"] = m_offsetLeft;
            map["baseline"] = m_baseline;

            return map;
        }
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const GlyphContext &profile);
    friend QDataStream & operator >> (QDataStream &in, GlyphContext &profile);
#endif


private:
    int m_glyphId;
    QChar m_character;
    int m_glyphSize;
    int m_profile;
    int m_offsetLeft;
    int m_baseline;
};

#ifndef QT_NO_DATASTREAM
    inline QDataStream & operator << (QDataStream &out, const GlyphContext &profile)
    {
        out << profile.m_glyphId
            << profile.m_character
            << profile.m_glyphSize
            << profile.m_profile
            << profile.m_offsetLeft
            << profile.m_baseline;

        return out;
    }

    inline QDataStream & operator >> (QDataStream &in, GlyphContext &profile)
    {
        in  >> profile.m_glyphId
            >> profile.m_character
            >> profile.m_glyphSize
            >> profile.m_profile
            >> profile.m_offsetLeft
            >> profile.m_baseline;

        return in;
    }
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const GlyphContext &glyph)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphContext(Id: "
        << glyph.id()
        << ", Size: "
        << glyph.size()
        << ", Character: "
        << glyph.character()
        << ", profile: "
        << glyph.profile()
        << ", Valid: "
        << glyph.isValid ()
        << ", offset left: "
        << glyph.offsetLeft()
        << ", baseline: "
        << glyph.baseline()
        << ")";

    return debug;
}
#endif

Q_DECLARE_METATYPE(GlyphContext)

#endif // GLYPHCONTEXT_H_
