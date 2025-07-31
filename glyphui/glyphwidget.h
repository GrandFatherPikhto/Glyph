#ifndef GLYPHWIDGET_H
#define GLYPHWIDGET_H

#include <QSharedPointer>
#include <QWidget>
#include <QObject>
#include <QChar>
#include <QPainter>
#include <QVector>
#include <QFont>
#include <QHash>
#include <QMargins>

#include "glyphui_global.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class AppSettings;
class AppContext;
class GlyphContext;
class GlyphManager;

namespace Ui {
class GlyphWidget;
}

class GLYPHUI_EXPORT GlyphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlyphWidget(AppContext *appContext, QWidget *parent = nullptr);
    ~GlyphWidget();

public slots:
    void setGlyphMeta (QSharedPointer<GlyphContext> context);

signals:

    // QWidget interface
protected:
    
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void setupSignals();
    void paintGrid (QPainter &painter);
    void paintBaseLines(QPainter &painter);
    void paintBitmapRect(QPainter &painter);
    void paintTemplateGlyph(QPainter &painter);
    void paintPreviewGlyph(QPainter &painter);
    void paintDrawGlyph(QPainter &painter);

    void initContext ();
    void initValues ();

    Ui::GlyphWidget *ui;

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;

    QMargins m_margins;

    QSharedPointer<GlyphContext> m_glyphContext;
};

#endif // GLYPHWIDGET_H
