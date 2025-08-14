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

#include "glyphcontext.h"
#include "profilecontext.h"

#include "GlyphDraw_global.h"

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

    void setProfile(const ProfileContext &context);
    void setGlyph(const GlyphContext &context);

    void setupSignals ();

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;

    GlyphContext m_glyph;
    ProfileContext m_profile;

    DrawContext *m_drawContext;
};

#endif // GLYPHDRAW_H
