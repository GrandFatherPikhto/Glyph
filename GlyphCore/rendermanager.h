#ifndef RENDERMANAGER_H_
#define RENDERMANAGER_H_

#include <QObject>

#include "iglyphrenderer.h"
#include "appcontext.h"
#include "glyphimage.h"

class RenderManager : public QObject {
    Q_OBJECT
public:
    RenderManager (AppContext *appContext, QObject *parent = nullptr);
    ~RenderManager ();
    
private:
    // Renderers
    QSharedPointer<IGlyphRenderer> m_ftRender; //< FreeType renderer
    QSharedPointer<IGlyphRenderer> m_drawRender; //< QImage renderer for draw layer

    GlyphKey m_key;
    
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_templates;
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_previews;
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_draws;

    AppContext *m_appContext;
};

#endif // RENDERMANAGER_H_