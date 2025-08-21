#ifndef GLYPHDRAWCONTEXT_H
#define GLYPHDRAWCONTEXT_H

#include <QWidget>
#include <QObject>
#include <QSize>
#include <QRect>
#include <QMargins>
#include <QPoint>
#include <QRegion>
#include <QSharedPointer>

#include "profilecontext.h"
#include "glyphcontext.h"
#include "imagecontext.h"
#include "drawcontext.h"

// #include "profilemanager.h"
// #include "glyphdrawcontext.h"
// #include "glyphmanager.h"
// #include "fontmanager.h"
// #include "imagemanager.h"

class AppContext;
class ProfileManager;
class AppSettings;
class GlyphManager;
class ImageManager;
class FontManager;

class GlyphDrawContext : public QObject
{
    Q_OBJECT
public:
    GlyphDrawContext(AppContext *appContext, QObject *parent);
    ~GlyphDrawContext();

    void setRegion(const QRegion &region);

    const QRect & drawRect() const { return m_drawRect; }

    ProfileContext glyphProfile();
    int dimX ();
    int dimY ();
    QList<QLine> gridHorizontalLines();
    QList<QLine> gridVerticalLines();
    int cellSize ();
    QPoint cellTopLeft(int col, int row);
    QPoint glyphTopLeft();
    QRect cellRect(int col, int row);
    QPoint bitmapTopLeft();
    QRect bitmapRect();
    bool clickGrid(const QPoint &point, int &col, int &row);

    const QRect & glyphRect() const { return m_glyphRect; }

    QSharedPointer<QImage> templateImage() const;
    QSharedPointer<QImage> previewImage() const;
    QSharedPointer<QImage> drawImage() const;

    bool clickBitmap(const QPoint &point, int &col, int &row);
    int baseLineY();
    int lineLeftX();

signals:
    void changeProfile(const ProfileContext &context);
    void changeMargins(const QMargins &margins);
    void changeGlyph(const GlyphContext &glyph);

private:
    void setupSignals();
    void setupValues ();

    void setProfile(const ProfileContext &context);
    void setGlyph(const GlyphContext &context);
    void setMargins(const QMargins &margins);

    bool renderTemplate();
    bool renderPreview();
    bool renderDraw();

    void updateContext();

    AppContext *m_appContext;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
    ImageManager *m_imageManager;
    FontManager *m_fontManager;

    QRect m_drawRect;
    QRect m_glyphRect;

    // ProfileContext m_profile;
    // GlyphContext m_glyph;
    
    QMargins m_margins;
    QRegion m_region;

    QSharedPointer<ImageContext> m_template;
    QSharedPointer<ImageContext> m_preview;
    QSharedPointer<DrawContext> m_draw;
};

#endif // GLYPHDRAWCONTEXT_H
