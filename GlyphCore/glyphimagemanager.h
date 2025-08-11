#ifndef IMAGEMANAGER_H_
#define IMAGERMANAGER_H_

#include <QObject>

#include "iglyphrenderer.h"
#include "imagekey.h"

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;
class GlyphImage;

class GLYPHCORE_EXPORT GlyphImageManager : public QObject {
    Q_OBJECT
public:

    GlyphImageManager (AppContext *appContext);
    ~GlyphImageManager ();

    QSharedPointer<GlyphImage> findOrRenderImage(QSharedPointer<GlyphContext> glyphContext, ImageKey::ImageType type, const QSize &size = QSize());
    void setAppSettings(AppSettings * appSettings);
    
private:

    QSharedPointer<GlyphImage> find (const ImageKey &key);
    bool remove(const ImageKey &key);

    // Renderers
    QSharedPointer<IGlyphRenderer> m_ftRender; //< FreeType renderer
    QSharedPointer<IGlyphRenderer> m_drawRender; //< QImage renderer for draw layer

    QHash<ImageKey, QSharedPointer<GlyphImage>> m_images;

    AppContext *m_appContext;
    AppSettings * m_appSettings;
};

#endif // IMAGEMANAGER_H_