#ifndef GLYPHDRAW_H
#define GLYPHDRAW_H

#include <QObject>
#include <QWidget>
#include <QPainter>

class AppContext;
class GlyphManager;
class AppSettings;
class ProfileManager;

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

    void drawGrid(QPainter &painter);

    void setProfile(const ProfileContext &context);
    void setGlyph(const GlyphContext &context);
    void paintEvent(QPaintEvent *event) override;


private:
    void setupSignals ();

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;

    GlyphContext m_glyph;
    ProfileContext m_profile;
};

#endif // GLYPHDRAW_H
