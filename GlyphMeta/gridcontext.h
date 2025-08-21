#ifndef GRIDCONTEXT_H
#define GRIDCONTEXT_H

#include <QDebug>
#include <QMetaType>
#include <QDataStream>
#include <QVariant>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GridContext
{
public:
    constexpr GridContext(int id = -1, int width = -1, int height = -1, int left = -1, int top = -1, int right = -1, int bottom = -1) 
        : m_id(id)
        , m_left(left)
        , m_top(top)
        , m_right(right)
        , m_bottom(bottom) 
        , m_width(width)
        , m_height(height)
    {}

    GridContext(const GridContext &grid)
        : m_id(grid.m_id)
        , m_left(grid.m_left)
        , m_top(grid.m_top)
        , m_right(grid.m_right)
        , m_bottom(grid.m_bottom)
        , m_width(grid.m_width)
        , m_height(grid.m_height)
        {}

    constexpr inline int id() const noexcept { return m_id; }
    constexpr inline int left() const noexcept { return m_left; }
    constexpr inline int top() const noexcept { return m_top; }
    constexpr inline int right() const noexcept { return m_right; }
    constexpr inline int bottom() const noexcept { return m_bottom; }
    constexpr inline int width() const { return m_width; }
    constexpr inline int height() const { return m_height; }
    constexpr inline int dimX() const { return m_left + m_width + m_right; }
    constexpr inline int dimY() const { return m_top + m_height + m_bottom; }

    constexpr inline bool isValid() const { return dimX() > 0 && dimY() > 0; }
    constexpr inline bool isEmpty() const { return m_width < 0 || m_height < 0; }

    constexpr inline void setId(int value = -1) noexcept;
    constexpr inline void setLeft(int value = 0) noexcept;
    constexpr inline void setTop(int value = 0) noexcept;
    constexpr inline void setRight(int value = 0) noexcept;
    constexpr inline void setBottom(int value = 0) noexcept;
    constexpr inline void setWidth(int value = 0) noexcept;
    constexpr inline void setHeight(int value = 0) noexcept;

    constexpr GridContext & operator = (const GridContext & value) noexcept;
    constexpr bool operator==(const GridContext& other) const noexcept;
    constexpr bool operator!=(const GridContext& other) const noexcept;

    // Для QVariant/QSettings
    operator QVariant() const {
        if(QMetaType::fromName("GridContext").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridContext не зарегистрирован
            QVariantMap map;
            map["id"] = m_id;
            map["left"] = m_left;
            map["top"] = m_top;
            map["right"] = m_right;
            map["bottom"] = m_bottom;
            map["width"] = m_width;
            map["height"] = m_height;

            return map;
        }
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream &operator<<(QDataStream &out, const GridContext &grid);
    friend QDataStream &operator>>(QDataStream &in, GridContext &grid);
#endif

private:
    int m_id;
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
    int m_width;
    int m_height;    
};

constexpr inline void GridContext::setId(int value) noexcept
{
    m_id = value;
}

constexpr inline void GridContext::setLeft(int value) noexcept
{
    m_left = value;
}

constexpr inline void GridContext::setTop(int value) noexcept
{
    m_top = value;
}

constexpr inline void GridContext::setRight(int value) noexcept
{
    m_right = value;
}

constexpr inline void GridContext::setBottom(int value) noexcept
{
    m_bottom = value;
}

constexpr inline void GridContext::setWidth(int value) noexcept
{
    m_width = value;
}

constexpr inline void GridContext::setHeight(int value) noexcept
{
    m_height = value;
}

#ifndef QT_NO_DATASTREAM
inline QDataStream &operator<<(QDataStream &out, const GridContext &grid)
{
    out << grid.m_id
        << grid.m_left
        << grid.m_top
        << grid.m_right
        << grid.m_bottom
        << grid.m_width
        << grid.m_height;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, GridContext &grid)
{
    in >> grid.m_id
       >> grid.m_left
       >> grid.m_top
       >> grid.m_right
       >> grid.m_bottom
       >> grid.m_width
       >> grid.m_height;

    return in;
}
#endif // QT_NO_DATASTREAM


constexpr bool GridContext::operator==(const GridContext& grid) const noexcept
{
    return    m_id == grid.m_id
           && m_left == grid.m_left
           && m_top == grid.m_top
           && m_right == grid.m_right
           && m_bottom == grid.m_bottom
           && m_width == grid.m_width
           && m_height == grid.m_height;
}

constexpr bool GridContext::operator!=(const GridContext& grid) const noexcept
{
    return    m_id != grid.m_id
           || m_left != grid.m_left
           || m_top != grid.m_top
           || m_right != grid.m_right
           || m_bottom != grid.m_bottom
           || m_width != grid.m_width
           || m_height != grid.m_height;
}

constexpr GridContext & GridContext::operator = (const GridContext & grid) noexcept
{
    m_id = grid.m_id;
    m_left = grid.m_left;
    m_top = grid.m_top;
    m_right = grid.m_right;
    m_bottom = grid.m_bottom;
    m_width = grid.m_width;
    m_height = grid.m_height;

    return *this;
}

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GridContext &grid)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GridContext( id: "
        << grid.id()
        << ", l:" << grid.left()
        << ", t:" << grid.top()
        << ", r:" << grid.right()
        << ", b:" << grid.bottom()
        << ", w:" << grid.width()
        << ", h:" << grid.height()
        << ")";

    return debug;
}
#endif

Q_DECLARE_METATYPE(GridContext)

#endif // GRIDCONTEXT_H
