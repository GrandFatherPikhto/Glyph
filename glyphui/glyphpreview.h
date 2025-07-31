#ifndef GLYPHPREVIEW_H
#define GLYPHPREVIEW_H

#include "glyphui_global.h"

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QSharedPointer>
#include <QMargins>

class AppContext;
class GlyphManager;
class AppSettings;
class GlyphContext;

class GLYPHUI_EXPORT GlyphPreview : public QWidget
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
    void initValues ();
    void setupSignals ();
    void initContext ();
    void paintGrid (QPainter &painter);

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;

    QSharedPointer<GlyphContext> m_glyphContext;

    // QRect m_glyphRect;
    // QRect m_renderRect;

    QMargins m_margins;

    int m_cellSize;
};

#endif // GLYPHPREVIEW_H
