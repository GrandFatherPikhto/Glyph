#ifndef GLYPHDRAWCONTEXT_H
#define GLYPHDRAWCONTEXT_H

#include <QWidget>
#include <QObject>
#include <QSize>
#include <QRect>
#include <QMargins>
#include <QPoint>
#include <QRegion>
#include <QSharedPointer>

#include "profilecontext.h"
#include "glyphcontext.h"
#include "imagecontext.h"
#include "drawcontext.h"

class AppContext;
class ProfileManager;
class AppSettings;
class GlyphManager;
class ImageManager;
class FontManager;

class GlyphDrawContext : public QObject
{
    Q_OBJECT
public:
    GlyphDrawContext(AppContext *appContext, QObject *parent);
    ~GlyphDrawContext();

    void setRegion(const QRegion &region);

    const QRect & drawRect() const { return m_drawRect; }

    inline int dimX () { return m_profile.paddingLeft() + m_profile.bitmapDimension() + m_profile.paddingRight(); }
    inline int dimY () { return m_profile.paddingTop() + m_profile.bitmapDimension() + m_profile.paddingBottom(); }

    inline QList<QLine> gridHorizontalLines()
    {
        QList<QLine> lines;
        for(int i = 0; i <= dimY(); i++)
        {
            QPoint left = cellTopLeft(0, i);
            QPoint right = cellTopLeft(dimX(), i);

            lines.append(QLine(left, right));
        }

        return lines;
    }

    inline QList<QLine> gridVerticalLines()
    {
        QList<QLine> lines;
        for(int i = 0; i <= dimX(); i++)
        {
            QPoint top = cellTopLeft(i, 0);
            QPoint bottom = cellTopLeft(i, dimY());

            lines.append(QLine(top, bottom));
        }

        return lines;
    }

    inline int cellSize () {
        int cellX = m_drawRect.width() / dimX();
        int cellY = m_drawRect.height() / dimY ();

        return cellX < cellY ? cellX : cellY;
    }

    inline QPoint cellTopLeft(int col, int row)
    {
        int size = cellSize();
        return m_drawRect.topLeft() + QPoint(col * size, row * size);
    }

    inline QPoint glyphTopLeft()
    {
        int size = cellSize();
        return m_drawRect.topLeft() + QPoint(m_profile.paddingLeft() * size, m_profile.paddingTop() * size);
    }

    inline QRect cellRect(int col, int row)
    {
        int size = cellSize();
        return QRect(cellTopLeft(col, row), QSize(size, size));
    }

    inline QPoint bitmapTopLeft()
    {
        int size = cellSize();
        return m_drawRect.topLeft() + QPoint(m_profile.paddingLeft() * size, m_profile.paddingTop() * size);
    }

    inline QRect bitmapRect()
    {
        int size = cellSize();
        int dim = m_profile.bitmapDimension();
        return QRect(bitmapTopLeft(), QSize(dim * size, dim * size));
    }

    inline bool clickGrid(const QPoint &point, int &col, int &row)
    {
        if (!m_drawRect.contains(point))
        {
            row = -1;
            col = -1;
            return false;
        }

        for (int x = 0; x < dimX(); x ++)
        {
            for (int y = 0; y < dimY(); y ++)
            {
                QRect rect = cellRect(x, y);
                if (rect.contains(point))
                {
                    col = x;
                    row = y;
                    return true;
                }
            }
        }

        return false;
    }

    inline const QRect & glyphRect() const { return m_glyphRect; }
    inline QSharedPointer<QImage> imageTemplate() const { return m_template->image(); }
    inline QSharedPointer<QImage> imagePreview() const { return m_preview->image(); }

    inline bool clickBitmap(const QPoint &point, int &col, int &row)
    {
        if (!bitmapRect().contains(point))
        {
            row = -1;
            col = -1;
            return false;
        }

        for (int x = 0; x < m_profile.bitmapDimension(); x ++)
        {
            for (int y = 0; y < m_profile.bitmapDimension(); y ++)
            {
                QRect rect = cellRect(x + m_profile.paddingLeft(), y + m_profile.paddingTop());
                if (rect.contains(point))
                {
                    col = x;
                    row = y;
                    return true;
                }
            }
        }

        return false;
    }

    inline int baseLineY()
    {
        int baseline = m_profile.bitmapDimension() + m_profile.paddingTop() - m_glyph.baseline();
        QPoint pointBaseline = cellTopLeft(0, baseline);
        return pointBaseline.y();
    }

    inline int lineLeftX()
    {
        int leftline = m_profile.paddingLeft() + m_glyph.offsetLeft();
        QPoint pointLeftLine = cellTopLeft(leftline, 0);        
        return pointLeftLine.x();
    }



signals:
    void changeProfile(const ProfileContext &context);
    void changeMargins(const QMargins &margins);
    void changeGlyph(const GlyphContext &glyph);

private:
    void setupSignals();
    void setupValues ();

    void setProfile(const ProfileContext &context);
    void setGlyph(const GlyphContext &context);
    void setMargins(const QMargins &margins);

    bool renderTemplate();
    bool renderPreview();
    bool renderDraw();

    AppContext *m_appContext;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
    ImageManager *m_imageManager;
    FontManager *m_fontManager;

    QRect m_drawRect;
    QRect m_glyphRect;

    ProfileContext m_profile;
    GlyphContext m_glyph;
    
    QMargins m_margins;
    QRegion m_region;

    QSharedPointer<ImageContext> m_template;
    QSharedPointer<ImageContext> m_preview;
    QSharedPointer<DrawContext> m_draw;
};

#endif // GLYPHDRAWCONTEXT_H
