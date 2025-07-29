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
    GlyphDimensions() noexcept {}

    void setGridPaddings(const GridPaddings &paddings);
    void setGlyphMarkup(const GlyphMarkup &markup);
    void setWidgetSize(const QSize &size);
    void setBitmapDimension(int value);
    void setMargins (const QMargins &value);

    bool isValid() const noexcept { return false; }

    int rows() const noexcept { return m_paddings.left() + m_paddings.right() + m_dimension; }
    int cols() const noexcept { return m_paddings.top() + m_paddings.bottom() + m_dimension; }
    const GridPaddings & paddings() const noexcept { return m_paddings; }

    int gridCellSize() const { return m_cellSize; };

    QPoint widgetBitmapLeftTop() const;
    QRect widgetGridRect() const;
    QRect widgetBitmapRect() const;
    bool widgetGridClick(const QPoint &click, int &row, int &col);

private:
    void calcCellSize ();
    void updateContext();

    QSize m_size;
    int m_dimension;
    int m_cellSize;
    GlyphMarkup m_glyphMarkup;
    GridPaddings m_paddings;
    QMargins m_margins;
};

void GlyphDimensions::updateContext()
{
    calcCellSize ();
}

constexpr void GlyphDimensions::setGridPaddings(const GridPaddings &paddings) noexcept
{
    m_paddings = paddings;
    updateContext();
}

void GlyphDimensions::calcCellSize()
{
    if (rows() <= 0 || cols() <= 0 || m_size.isEmpty())
    {
        m_cellSize = -1;
        return;
    }

    int cellX = m_size.width() / cols();
    int cellY = m_size.height() / rows();

    m_cellSize = cellX < cellY ? cellX : cellY;
}

QPoint GlyphDimensions::widgetBitmapLeftTop() const
{
    if (!isValid() || m_size.isEmpty())
        return QPoint();

    if (m_cellSize <= 0)
        return QPoint();
    
    return QPoint(m_margins.left() + m_paddings.left() * m_cellSize, m_margins.top() + m_paddings.top() * m_cellSize);
}

QRect GlyphDimensions::widgetGridRect() const
{
    if (!isValid() || m_size.isEmpty())
        return QRect();

    return QRect(QPoint(m_margins.left(),m_margins.top()), QSize(rows() * m_cellSize, cols() * m_cellSize));
}

inline constexpr QRect GlyphDimensions::widgetBitmapRect() const
{
    if (!isValid() || m_size.isEmpty())
        return QRect();

    return QRect(widgetBitmapLeftTop(), QSize(m_dimension * m_cellSize, m_dimension * m_cellSize));
}

inline constexpr bool GlyphDimensions::widgetGridClick(const QPoint &click, int &row, int &col)
{
    QRect bitmapRect = widgetBitmapRect(size);
    if(bitmapRect.isEmpty())
        return false;
    bitmapRect.translated(leftTop);
    if (!bitmapRect.contains(click))
        return false;

    int cellSize = widgetCellSize(size);
    if (cellSize <= 0)
        return false;
    row = -1;
    col = -1;

    for (int x = 0; x < m_dimension; x++)
    {
        for (int y = 0; y < m_dimension; y++ )
        {
            QRect cellRect(QPoint(m_paddings.left() * cellSize, m_paddings.top() * cellSize), QSize(x * cellSize, y * cellSize));
            cellRect.translated(leftTop);
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


#if 0
inline QDebug operator<<(QDebug debug, const GlyphDimensions &grid)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GridDimensions("
                   << "dimension=" << grid.dimension()
                   << " , (" << grid.paddings().left()
                   << ", " << grid.paddings().top()
                   << ", " << grid.paddings().right()
                   << ", " << grid.paddings().bottom()
                   << ")";
    return debug;
}
#endif // QT_NO_DEBUG_OUTPUT

Q_DECLARE_METATYPE(GlyphDimensions)

#endif // GLYPHGRID_H_