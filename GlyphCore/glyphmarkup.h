#ifndef GLYPHMARKUP_H
#define GLYPHMARUP_H

#include "gridpaddings.h"

#include <QRect>

class GlyphMarkup
{
public:
    constexpr GlyphMarkup(int width, int height, int dimension, int left = 0, int top = 0) 
        : m_width(width), m_height(height), m_dimension(dimension), m_left(left), m_top(top) 
        {};

    // constexpr inline GlyphMarkup &operator+=(const GridPaddings &paddings) noexcept;

    constexpr inline int width() const noexcept { return m_width; }
    constexpr inline int height() const noexcept { return m_height; }
    constexpr inline int dimension() const noexcept { return m_dimension; }

    constexpr inline void setWidth(int value) noexcept;
    constexpr inline void setHeight(int value) noexcept;
    constexpr inline void setDimension(int value) noexcept;
    constexpr inline void setLeft(int value) noexcept;
    constexpr inline void setTop(int value) noexcept;

private:
    int m_width;
    int m_height;
    int m_dimension;
    int m_left;
    int m_top;

    QRect rect;
};

constexpr inline void GlyphMarkup::setWidth(int value) noexcept
{
    m_width = value;
}

constexpr inline void GlyphMarkup::setHeight(int value) noexcept
{
    m_height = value;
}

constexpr inline void GlyphMarkup::setDimension(int value) noexcept
{
    m_dimension = value;
}

constexpr inline void GlyphMarkup::setLeft(int value) noexcept
{
    m_left = value;
}

constexpr inline void GlyphMarkup::setTop(int value) noexcept
{
    m_top = value;
}

#endif // GLYPHMARKUP_H
