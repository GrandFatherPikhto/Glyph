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
#include "glyph.h"

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
    explicit GlyphWidget(GlyphManager *glyphManager, QWidget *parent = nullptr);
    ~GlyphWidget();

public slots:
    // void setGlyphSize(int newFontPtSize);
    void setGlyph (QSharedPointer<Glyph> newGlyph);
    void enableGeneratedGlyph(bool editable);
    void enableGrid(bool enable);
    void enableContour(bool enable);
    void enableGlyphGrid(bool enable);

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void loadFontFace();
    void mousePressEvent(QMouseEvent *event);

private:
    void calcRenderRect ();
    int glyphKeyEntry ();
    void renderGlyphImage();
    void renderGlyphPixels ();
    int findOptimalGlyphSize ();
    void resetGlyphPixels ();
    void updateGlyph ();

    Ui::GlyphWidget *ui;

    GlyphManager *m_glyphManager;

    FT_Library m_ftLib;
    FT_Face m_ftFace;
    QImage *m_glyphImage;
    int m_gridCellSize;

    bool m_generatedGlyphEnable;
    bool m_gridEnable;
    bool m_countourEnable;
    bool m_glyphGridEnable;

    QRect m_glyphRect;
    QRect m_renderRect;

    QSharedPointer<Glyph> m_glyph;
};

#endif // GLYPHWIDGET_H
