#ifndef GLYPHOFFSET_H_
#define GLYPHOFFSET_H_

#include <QDebug>
#include <QDataStream>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GlyphOffset {
public:
    constexpr GlyphOffset(int offsetX = 0, int offsetY = 0) : m_offsetX(offsetX), m_offsetY(offsetY) {}
    ~GlyphOffset() {}

    constexpr inline void setOffsetX (int value) noexcept;
    constexpr inline void setOffsetY (int value) noexcept;

    constexpr inline int offsetX () const noexcept { return m_offsetX; }
    constexpr inline int offsetY () const noexcept { return m_offsetY; }

    constexpr inline void reset () noexcept { m_offsetX = 0; m_offsetY = 0; }
    constexpr inline int  up ()    noexcept { return ++ m_offsetY; }
    constexpr inline int  down ()  noexcept { return -- m_offsetY; }
    constexpr inline int  left ()  noexcept { return -- m_offsetX; }
    constexpr inline int  right () noexcept { return ++ m_offsetX; }

    constexpr bool operator==(const GlyphOffset& offset) const noexcept;
    constexpr bool operator!=(const GlyphOffset& offset) const noexcept;

    constexpr inline GlyphOffset & operator = (const GlyphOffset & offset) noexcept;
    friend inline GlyphOffset operator * (const GlyphOffset & offset, int value);

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphOffset &offset);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphOffset &offset);
#endif

private:
    int m_offsetX;
    int m_offsetY;
};

constexpr inline void GlyphOffset::setOffsetX(int value) noexcept
{
    m_offsetX = value;
}

constexpr inline void GlyphOffset::setOffsetY(int value) noexcept
{
    m_offsetY = value;
}

constexpr bool GlyphOffset::operator==(const GlyphOffset& offset) const noexcept
{
    return (
        m_offsetX == offset.m_offsetX
        && m_offsetY == offset.m_offsetY
    );
}

constexpr bool GlyphOffset::operator!=(const GlyphOffset& offset) const noexcept
{
    return (
        m_offsetX != offset.m_offsetX
        && m_offsetY != offset.m_offsetY
    );
}

constexpr inline GlyphOffset & GlyphOffset::operator = (const GlyphOffset & offset) noexcept
{
    m_offsetX = offset.m_offsetX;
    m_offsetY = offset.m_offsetY;

    return *this;
}

inline GlyphOffset operator* (const GlyphOffset &offset, int value)
{
    return GlyphOffset(offset.m_offsetX * value, offset.m_offsetY * value);
}

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphOffset &offset)
{
    out << offset.m_offsetX
        << offset.m_offsetY;

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphOffset &offset)
{
    in >> offset.m_offsetX
       >> offset.m_offsetY;

    return in;
}
#endif 

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphOffset &offset)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphOffset("
        << offset.offsetX()
        << ", "
        << offset.offsetY()
        << ")";

    return debug;
}
#endif

#endif // GLYPHOFFSET_H_