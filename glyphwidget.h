#ifndef GLYPHWIDGET_H
#define GLYPHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QChar>
#include <QPainter>
#include <QVector>
#include <QFont>

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

public slots:
    void setGlyphFont(const QFont &newFont, const QString &newFontPath);
    void setGridSize(int newGridSize);
    void setGlyphSize(int newFontPtSize);
    void setCharacter(const QChar &newCharacter);

    void setGlyphParams(const QFont &newFont, const QString & newFontPath, int newFontSize, const QChar &newCharacter, int gridSize);

signals:

private:
    void renderGlyphImage();
    void renderGlyphPixels ();
    int findOptimalGlyphSize ();
    void resetGlyphPixels ();

    void updateGlyph ();

    FT_Library m_ftLib;
    FT_Face m_ftFace;

    QFont m_font;
    int m_glyphSize;
    QString m_fontPath;
    QChar m_character;
    QImage *m_glyphImage;
    QVector<bool> m_glyphPixels;

    int m_gridSize;
    int m_glyphRows;
    int m_glyphCols;

    int m_gridRows;
    int m_gridCols;
    int m_gridCellSize;
    
    QRect m_glyphRect;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // GLYPHWIDGET_H
