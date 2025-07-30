#ifndef RENDERMANAGER_H_
#define RENDERMANAGER_H_

#include <QObject>

#include "iglyphrenderer.h"

class RenderManager : public QObject {
    Q_OBJECT
public:
    RenderManager () {}
    ~RenderManager () {};
    
private:
    // Renderers
    QSharedPointer<IGlyphRenderer> m_ftRender; //< FreeType renderer
    QSharedPointer<IGlyphRenderer> m_drawRender; //< QImage renderer for draw layer
};

#endif RENDERMANAGER_H_