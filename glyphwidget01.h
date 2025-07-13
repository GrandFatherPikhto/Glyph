#ifndef GLYPHWIDGET_H
#define GLYPHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QChar>
#include <QPainter>
#include <QVector>
#include <QFont>
#include <QHash>

#include "glyphkey.h"
#include "glyphentry.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


class GlyphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlyphWidget(QWidget *parent = nullptr);
    ~GlyphWidget();

public slots:
    void setGlyphFont(const QFont &newFont, const QString &newFontPath);
    void setGridSize(int newGridSize);
    void setGlyphSize(int newFontPtSize);
    void setCharacter(const QChar &newCharacter);

    void setGlyphParams(const QFont &newFont, const QString & newFontPath, int newFontSize, const QChar &newCharacter, int gridSize);

    void moveGlyphLeft();
    void moveGlyphTop();
    void moveGlyphRight();
    void moveGlyphDown();
    void moveGlyphCenter();

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    int glyphKeyEntry ();
    void renderGlyphImage();
    void renderGlyphPixels ();
    int findOptimalGlyphSize ();
    void resetGlyphPixels ();
    void calcGlyphRect ();

    void updateGlyph ();

    FT_Library m_ftLib;
    FT_Face m_ftFace;

    // QFont m_font;
    // int m_glyphSize;
    // QString m_fontPath;
    // QChar m_character;
    QImage *m_glyphImage;
    // QVector<bool> m_glyphPixels;

    // int m_gridSize;
    // int m_glyphRows;
    // int m_glyphCols;

    // int m_gridRows;
    // int m_gridCols;
    int m_gridCellSize;
    
    // QRect m_glyphRect;

    QHash<GlyphKey, GlyphEntry> m_glyphCache;
    GlyphKey m_glyphKey;
    GlyphEntry m_glyphEntry;
};

#endif // GLYPHWIDGET_H
