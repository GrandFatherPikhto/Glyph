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

class GlyphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlyphWidget(AppContext *appContext, QWidget *parent = nullptr);
    ~GlyphWidget();

public slots:
    // void setGlyphSize(int newFontPtSize);
    void setGlyphMeta (QSharedPointer<GlyphMeta> newGlyph);
    void enableTemplateLayer(bool editable);
    void enableGrid(bool enable);
    void enablePreviewLayer(bool enable);
    void enableUserLayer(bool enable);
    void enableGlyphRectLayer(bool enable);
    void enableBitmapRectLayer(bool enable);
    void enableBaselineLayer(bool enable);
    void setLeftGridCells(int value);
    void setBottomGridCells(int value);
    void pasteGlyphToUserLayer();
    void clearUserLayer ();

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void paintGrid (QPainter &painter);
    void paintBaseLines(QPainter &painter);
    void paintBitmapRect(QPainter &painter);
    void paintTemplateGlyph(QPainter &painter);
    void paintPreviewGlyph(QPainter &painter);
    void paintUserGlyph(QPainter &painter);

    void initContext ();

    Ui::GlyphWidget *ui;

    AppContext *m_appContext;
    QSharedPointer<QImage> m_userGlyph;
    QSharedPointer<QImage> m_templateGlyph;
    QSharedPointer<QImage> m_previewGlyph;

    int m_gridCellSize;
    int m_padding;
    int m_leftCells;
    int m_bottomCells;
    int m_xGridCells;
    int m_yGridCells;

    bool m_templateLayerEnable;
    bool m_gridLayerEnable;
    bool m_previewLayerEnable;
    bool m_userLayerEnable;
    bool m_glyphRectEnable;
    bool m_baselineEnable;
    bool m_bitmapRectEnable;

    QRect m_renderRect;
    QRect m_glyphRect;
    QRect m_gridRect;

    QSharedPointer<GlyphMeta> m_glyphMeta;
};

#endif // GLYPHWIDGET_H
