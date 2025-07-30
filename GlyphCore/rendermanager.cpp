#include "rendermanager.h"
#include "freetypeglyphrenderer.h"
#include "drawglyphrenderer.h"

RenderManager::RenderManager()
    : m_ftRender(QSharedPointer<IGlyphRenderer>())
    , m_drawRender(QSharedPointer<IGlyphRenderer>())
{
    m_ftRender = QSharedPointer<FreeTypeGlyphRenderer>::create();
    m_drawRender = QSharedPointer<DrawGlyphRenderer>::create();
}

RenderManager::~RenderManager()
{

}