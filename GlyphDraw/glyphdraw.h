#ifndef GLYPHDRAW_H
#define GLYPHDRAW_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>

class AppContext;
class GlyphManager;
class AppSettings;
class ProfileManager;
class DrawContext;
class ImageManager;
class FontManager;
class GridManager;

#include "glyphcontext.h"
#include "profilecontext.h"
#include "gridcontext.h"

#include "GlyphDraw_global.h"

class GlyphDrawContext;

class GLYPHDRAW_EXPORT GlyphDraw : public QWidget
{
    Q_OBJECT
public:
    GlyphDraw(QWidget *parent = nullptr);
    ~GlyphDraw();

    void setAppContext(AppContext *appContext);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    void drawGrid(QPainter &painter);
    void drawBitmapRect(QPainter &painter);
    void drawBaseLine(QPainter &painter);
    void drawLeftLine(QPainter &painter);
    void drawTemplate(QPainter &painter);
    void drawPreview(QPainter &painter);

    // bool renderTemplate(const GlyphContext &glyph);
    // bool renderPreview(const GlyphContext &glyph);

    void setProfile(const ProfileContext &context);
    void setGlyph(const GlyphContext &context);
    void setGrid(const GridContext &context);

    void setupSignals ();

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    ImageManager *m_imageManager;
    FontManager *m_fontManager;
    GridManager *m_gridManager;

    GlyphContext m_glyph;
    ProfileContext m_profile;

    GlyphDrawContext *m_drawContext;
};

#endif // GLYPHDRAW_H
