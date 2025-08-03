#ifndef FTGLYPH_H_
#define FTGLYPH_H_

#include <QPoint>
#include <QRect>
#include <QMetaType>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT FTGlyph {
public:
    constexpr FTGlyph(int left, int top, int rows, int width) noexcept : m_left(left), m_top(top), m_rows(rows), m_width(width) { };
    
    constexpr int left() { return m_left; }
    constexpr int top() { return m_left; }
    constexpr int rows() { return m_left; }
    constexpr int width() { return m_left; }

    constexpr inline void setLeft(int pos) noexcept;
    constexpr inline void setTop(int pos) noexcept;
    constexpr inline void setRows(int pos) noexcept;
    constexpr inline void setWidth(int pos) noexcept;


private:
    int m_left;
    int m_top;
    int m_rows;
    int m_width;
};

constexpr inline void FTGlyph::setLeft(int pos) noexcept
{ m_left = pos; }
constexpr inline void FTGlyph::setTop(int pos) noexcept
{ m_top = pos; }
constexpr inline void FTGlyph::setWidth(int pos) noexcept
{ m_width = pos; }
constexpr inline void FTGlyph::setRows(int pos) noexcept
{ m_rows = pos; }

Q_DECLARE_METATYPE(FTGlyph)

#endif // FTGLYPH_H_
