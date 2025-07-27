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

#include "glyphmanager.h"
#include "appcontext.h"
#include "glyphmeta.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

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
    void setGlyphMeta (QSharedPointer<GlyphMeta> newGlyph);

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

    int m_gridCellSize;
    QMargins m_margins;
    int m_leftCells;
    int m_bottomCells;
    int m_xGridCells;
    int m_yGridCells;

    bool m_templateLayerEnable;
    bool m_gridLayerEnable;
    bool m_previewLayerEnable;
    bool m_drawLayerEnable;
    bool m_glyphRectEnable;
    bool m_baselineEnable;
    bool m_bitmapRectEnable;

    QRect m_renderRect;
    QRect m_glyphRect;
    QRect m_gridRect;

    QSharedPointer<GlyphMeta> m_glyphMeta;
};

#endif // GLYPHWIDGET_H
