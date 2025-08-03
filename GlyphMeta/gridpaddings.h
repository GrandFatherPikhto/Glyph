#ifndef GRIDPADDINGS_H
#define GRIDPADDINGS_H

#include <QDebug>
#include <QMetaType>
#include <QDataStream>
#include <QVariant>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GridPaddings
{
public:
    constexpr GridPaddings(int left = 0, int top = 0, int right = 0, int bottom = 0) : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {}
    GridPaddings(const GridPaddings &paddings)
        : m_left(paddings.m_left)
        , m_top(paddings.m_top)
        , m_right(paddings.m_right)
        , m_bottom(paddings.m_bottom)
        {}

    constexpr GridPaddings & operator = (const GridPaddings & value) noexcept;

    constexpr inline int left() const noexcept { return m_left; }
    constexpr inline int top() const noexcept { return m_top; }
    constexpr inline int right() const noexcept { return m_right; }
    constexpr inline int bottom() const noexcept { return m_bottom; }

    constexpr inline void setLeft(int value) noexcept;
    constexpr inline void setTop(int value) noexcept;
    constexpr inline void setRight(int value) noexcept;
    constexpr inline void setBottom(int value) noexcept;

    constexpr bool operator==(const GridPaddings& other) const noexcept;
    constexpr bool operator!=(const GridPaddings& other) const noexcept;

    // Для QVariant/QSettings
    operator QVariant() const {
        if(QMetaType::fromName("GridPaddings").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridPaddings не зарегистрирован
            QVariantMap map;
            map["width"] = m_left;
            map["height"] = m_top;
            map["dimension"] = m_right;
            map["left"] = m_bottom;

            return map;
        }
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream &operator<<(QDataStream &out, const GridPaddings &paddings);
    friend QDataStream &operator>>(QDataStream &in, GridPaddings &paddings);
#endif

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;    
};

constexpr inline void GridPaddings::setLeft(int value) noexcept
{
    m_left = value;
}

constexpr inline void GridPaddings::setTop(int value) noexcept
{
    m_top = value;
}

constexpr inline void GridPaddings::setRight(int value) noexcept
{
    m_right = value;
}

constexpr inline void GridPaddings::setBottom(int value) noexcept
{
    m_bottom = value;
}

#ifndef QT_NO_DATASTREAM
inline QDataStream &operator<<(QDataStream &out, const GridPaddings &paddings)
{
    out << paddings.m_left
        << paddings.m_top
        << paddings.m_right
        << paddings.m_bottom;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, GridPaddings &paddings)
{
    in >> paddings.m_left
       >> paddings.m_top
       >> paddings.m_right
       >> paddings.m_bottom;

    return in;
}
#endif // QT_NO_DATASTREAM


constexpr bool GridPaddings::operator==(const GridPaddings& paddings) const noexcept
{
    return m_left == paddings.m_left &&
           m_top == paddings.m_top &&
           m_right == paddings.m_right &&
           m_bottom == paddings.m_bottom;
}

constexpr bool GridPaddings::operator!=(const GridPaddings& paddings) const noexcept
{
    return !(*this == paddings);
}

constexpr GridPaddings & GridPaddings::operator = (const GridPaddings & paddings) noexcept
{
    m_left = paddings.m_left;
    m_top = paddings.m_top;
    m_right = paddings.m_right;
    m_bottom = paddings.m_bottom;

    return *this;
}

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GridPaddings &paddings)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GridPaddings("
        << paddings.left()
        << ", " << paddings.top()
        << ", " << paddings.right()
        << ", " << paddings.bottom()
        << ")";

    return debug;
}
#endif

Q_DECLARE_METATYPE(GridPaddings)

#endif // GRIDPADDINGS_H
