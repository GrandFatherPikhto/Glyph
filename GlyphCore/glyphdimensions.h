#ifndef GLYPHGRID_H_
#define GLYPHGRID_H_

#include <QSize>
#include <QRect>
#include <QDebug>
#include <QMetaType>
#include <QDataStream>

#include "gridpaddings.h"
#include "glyphmarkup.h"

class GlyphDimensions {
public:
    GlyphDimensions() 
        : m_gridRect(QRect())
        , m_bitmapRect(QRect())
        , m_glyphRect(QRect())
        , m_size(QSize())
        , m_dimension(-1)
        , m_margins(QMargins())
        , m_glyphMarkup(GlyphMarkup())
        , m_cellSize(-1)
        {}

    ~GlyphDimensions() {}

    void setGridPaddings(const GridPaddings &paddings);
    void setGlyphMarkup(const GlyphMarkup &markup);
    void setWidgetSize(const QSize &size);
    void setBitmapDimension(int value);
    void setMargins (const QMargins &value);

    bool isValid() const noexcept { return !(m_size.isEmpty() || m_cellSize <= 0 || m_dimension <= 6); }

    int rows() const noexcept { return m_paddings.left() + m_paddings.right() + m_dimension; }
    int cols() const noexcept { return m_paddings.top() + m_paddings.bottom() + m_dimension; }
    const GridPaddings & paddings() const noexcept { return m_paddings; }

    int gridCellSize() const { return m_cellSize; };

    QRect widgetGridRect() const { return m_gridRect; }
    QRect widgetBitmapRect() const { return m_bitmapRect; }
    QRect widgetGlyphRect () const { return m_glyphRect; }

    bool widgetGridClick(const QPoint &click, int &row, int &col);

private:
    void calcWidgetCellSize   ();
    void calcWidgetGridRect   ();
    void calcWidgetBitmapRect ();
    void calcWidgetGlyphRect  ();

    void resetContext ();
    void updateContext();

    QSize m_size;
    int m_dimension;
    int m_cellSize;

    QRect m_gridRect;
    QRect m_glyphRect;
    QRect m_bitmapRect;

    GlyphMarkup m_glyphMarkup;
    GridPaddings m_paddings;
    QMargins m_margins;
};

void GlyphDimensions::setGlyphMarkup(const GlyphMarkup &markup)
{
    m_glyphMarkup = markup;
    updateContext();
}

void GlyphDimensions::setWidgetSize(const QSize &size)
{
    m_size = size;
    updateContext ();
}

void GlyphDimensions::setGridPaddings(const GridPaddings &paddings)
{
    m_paddings = paddings;
    updateContext ();
}

void GlyphDimensions::setMargins(const QMargins &margins)
{
    m_margins = margins;
    updateContext ();
}

void GlyphDimensions::setBitmapDimension(int value)
{
    m_dimension = value;
    updateContext ();
}

void GlyphDimensions::resetContext ()
{
    m_cellSize = -1;
    m_gridRect = QRect   ();
    m_bitmapRect = QRect ();
    m_glyphRect  = QRect ();
}

void GlyphDimensions::updateContext()
{
    resetContext ();

    calcWidgetCellSize   ();
    calcWidgetGridRect   ();
    calcWidgetBitmapRect ();
    calcWidgetGlyphRect  ();
}

constexpr void GlyphDimensions::setGridPaddings(const GridPaddings &paddings) noexcept
{
    m_paddings = paddings;
    updateContext();
}

void GlyphDimensions::calcWidgetCellSize()
{
    if (!isValid())
        return;

    int cellX = m_size.width() / cols();
    int cellY = m_size.height() / rows();

    m_cellSize = cellX < cellY ? cellX : cellY;
}

void GlyphDimensions::calcWidgetGridRect()
{
    if (!isValid())
        return;

    m_gridRect = QRect(QPoint(m_margins.left(),m_margins.top()), QSize(rows() * m_cellSize, cols() * m_cellSize));
}

void GlyphDimensions::calcWidgetBitmapRect()
{
    if (!isValid())
        return;

    m_bitmapRect = QRect(m_glyphRect.topLeft(), QSize(m_dimension * m_cellSize, m_dimension * m_cellSize));
}

void GlyphDimensions::calcWidgetGlyphRect ()
{
    if (!isValid())
        return;

    m_glyphRect = QRect((m_gridRect.topLeft() + QPoint()), QSize());
}

bool GlyphDimensions::widgetGridClick(const QPoint &click, int &row, int &col)
{
    if (!isValid() || m_bitmapRect.isEmpty() || m_gridRect.isEmpty())
        return false;

    if (!m_bitmapRect.contains(click))
        return false;

    row = -1;
    col = -1;

    for (int x = 0; x < m_dimension; x++)
    {
        for (int y = 0; y < m_dimension; y++ )
        {
            QRect cellRect(
                (m_bitmapRect.topLeft() + QPoint(x * m_cellSize, y * m_cellSize)), 
                QSize(m_cellSize, m_cellSize));
            
            if (cellRect.contains(click))
            {
                row = x;
                col = y;
                return true;
            }
        }
    }

    return false;
}


Q_DECLARE_METATYPE(GlyphDimensions)

#endif // GLYPHGRID_H_