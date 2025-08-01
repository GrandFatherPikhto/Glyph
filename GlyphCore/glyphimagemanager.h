#ifndef IMAGEMANAGER_H_
#define IMAGERMANAGER_H_

#include <QObject>

#include "iglyphrenderer.h"
#include "imagekey.h"

class AppSettings;
class GlyphImage;

class GlyphImageManager : public QObject {
    Q_OBJECT
public:

    GlyphImageManager (QObject *parent = nullptr);
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

    AppSettings * m_appSettings;
};

#endif // IMAGEMANAGER_H_