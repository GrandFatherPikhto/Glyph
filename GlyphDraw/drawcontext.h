#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H

#include <QWidget>
#include <QObject>
#include <QSize>
#include <QRect>
#include <QMargins>
#include <QPoint>
#include <QRegion>

#include "profilecontext.h"

class AppContext;
class ProfileManager;
class AppSettings;

class DrawContext : public QObject
{
    Q_OBJECT
public:
    DrawContext(AppContext *appContext, QObject *parent);
    ~DrawContext();

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

signals:
    void changeProfile(const ProfileContext &context);
    void changeMargins(const QMargins &margins);

private:
    void setupSignals();
    void setupValues ();

    void setProfile(const ProfileContext &context);
    void setMargins(const QMargins &margins);


    AppContext *m_appContext;
    ProfileManager *m_profileManager;
    AppSettings *m_appSettings;

    QRect m_drawRect;

    ProfileContext m_profile;
    QMargins m_margins;
    QRegion m_region;
};

#endif // DRAWCONTEXT_H
