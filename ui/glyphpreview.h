#ifndef GLYPHPREVIEW_H
#define GLYPHPREVIEW_H

#include "appcontext.h"
#include "glyphmeta.h"

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QSharedPointer>
#include <QMargins>

class GlyphPreview : public QWidget
{
    Q_OBJECT
public:
    explicit GlyphPreview(AppContext *appContext, QWidget *parent = nullptr);

public slots:


signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    void initContext ();
    void paintGrid (QPainter &painter);

    AppContext *m_appContext;
    QSharedPointer<GlyphMeta> m_glyphMeta;

    QRect m_glyphRect;
    QRect m_renderRect;

    QMargins m_margins;

    int m_cellSize;
};

#endif // GLYPHPREVIEW_H
