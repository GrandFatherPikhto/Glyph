#ifndef GLYPHMARKUP_H
#define GLYPHMARKUP_H

#include "gridpaddings.h"
#include "glyphoffset.h"

#include <QRect>
#include <QDataStream>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GlyphMarkup
{
public:
    constexpr GlyphMarkup(int width = -1, int height = -1, int left = 0, int top = 0) 
        : m_width(width), m_height(height), m_left(left), m_top(top) {}

    GlyphMarkup(const GlyphMarkup &markup)
        : m_left(markup.left())
        , m_top(markup.top())
        , m_width(markup.m_width)
        , m_height(markup.height())
        {}

    constexpr inline int width() const noexcept { return m_width; }
    constexpr inline int height() const noexcept { return m_height; }
    constexpr inline int left() const noexcept { return m_left; }
    constexpr inline int top() const noexcept { return m_top; }
    constexpr inline int leftline(const GlyphOffset &offset) noexcept;
    constexpr inline int baseline(const GlyphOffset &offset, int dimension) noexcept;

    constexpr inline void setWidth(int value) noexcept;
    constexpr inline void setHeight(int value) noexcept;
    constexpr inline void setLeft(int value) noexcept;
    constexpr inline void setTop(int value) noexcept;
    constexpr inline void setOffsetX(int value) noexcept;
    constexpr inline void setOffsetY(int value) noexcept;

    constexpr bool operator==(const GlyphMarkup& markup) const noexcept;
    constexpr GlyphMarkup & operator = (const GlyphMarkup & value) noexcept;

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const GlyphMarkup &markup);
    friend QDataStream & operator >> (QDataStream &in, GlyphMarkup &markup);
#endif

private:
    int m_width;
    int m_height;
    int m_left;
    int m_top;
};

constexpr inline int GlyphMarkup::baseline(const GlyphOffset &offset, int dimension) noexcept
{
    return dimension - (m_height - m_top) + offset.offsetY();
}

constexpr inline int GlyphMarkup::leftline(const GlyphOffset &offset) noexcept
{
    return m_left + offset.offsetX();
}

constexpr inline void GlyphMarkup::setWidth(int value) noexcept
{
    m_width = value;
}

constexpr inline void GlyphMarkup::setHeight(int value) noexcept
{
    m_height = value;
}

constexpr inline void GlyphMarkup::setLeft(int value) noexcept
{
    m_left = value;
}

constexpr inline void GlyphMarkup::setTop(int value) noexcept
{
    m_top = value;
}

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphMarkup &markup)
{
    out << markup.m_width
        << markup.m_height
        << markup.m_left
        << markup.m_top;

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphMarkup &markup)
{
    in >> markup.m_width
       >> markup.m_height
       >> markup.m_left
       >> markup.m_top;

    return in;
}
#endif 

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphMarkup &markup)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphMarkup("
        << markup.left()
        << ", " << markup.top()
        << " (" << markup.width()
        << " x " << markup.height()
        << "))";

    return debug;
}
#endif

constexpr GlyphMarkup & GlyphMarkup::operator = (const GlyphMarkup & markup) noexcept
{
    m_left = markup.m_left;
    m_top = markup.m_top;
    m_height = markup.m_height;
    m_width = markup.m_width;

    return *this;
}

constexpr bool GlyphMarkup::operator==(const GlyphMarkup& markup) const noexcept
{
    return m_width == markup.m_width
           && m_height == markup.m_height
           && m_left == markup.m_left
           && m_top == markup.m_top;
}

Q_DECLARE_METATYPE(GlyphMarkup)

#endif // GLYPHMARKUP_H
