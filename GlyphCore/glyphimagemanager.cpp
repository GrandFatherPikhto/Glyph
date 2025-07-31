#include "glyphimagemanager.h"
#include "glyphimage.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"
#include "appsettings.h"
#include "appcontext.h"

GlyphImageManager::GlyphImageManager(AppContext *appContext)
    : QObject(appContext)
    , m_ftRender(QSharedPointer<IGlyphRenderer>())
    , m_drawRender(QSharedPointer<IGlyphRenderer>())
{
    m_ftRender = QSharedPointer<FreeTypeGlyphRenderer>::create();
    m_drawRender = QSharedPointer<DrawGlyphRenderer>::create();
}

GlyphImageManager::~GlyphImageManager()
{

}

QSharedPointer<GlyphImage> GlyphImageManager::findOrRenderImage(QSharedPointer<GlyphContext> glyphContext, ImageKey::ImageType type, const QSize &size)
{
    ImageKey key(glyphContext->character(), glyphContext->dimension(), type);
    QSharedPointer<GlyphImage> image = find(key);

    AppContext *appContext = dynamic_cast<AppContext*>(parent());
    AppSettings *appSettings = appContext->appSettings();

    if (image.isNull())
    {
        image = QSharedPointer<GlyphImage>::create(glyphContext);
        m_images.insert(key, image);
    }

    if (!image.isNull() && image->isDirty())
    {
        switch (type)
        {
        case ImageKey::ImageTemplate:
            image->renderImage(m_ftRender, appSettings->templateColor(), appSettings->templateBgColor());
            break;

        case ImageKey::ImagePreview:            
            image->renderImage(m_ftRender, appSettings->previewColor(), appSettings->previewBgColor(), size);
            break;
        
        case ImageKey::ImageDraw:
            image->renderImage(m_drawRender, appSettings->previewColor(), appSettings->previewBgColor(), size);
            break;
        default:
            break;
        }
    }

    return image;
}

QSharedPointer<GlyphImage> GlyphImageManager::find(const ImageKey &key)
{
    auto it = m_images.find(key);
    if (it == m_images.end())
        return QSharedPointer<GlyphImage>();
    
    return it.value();
}

bool GlyphImageManager::remove (const ImageKey &key)
{
    auto it = m_images.find(key);
    if (it == m_images.end())
        return false;
    m_images.remove(key);
    return true;
}
