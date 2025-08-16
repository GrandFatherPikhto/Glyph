#include "ftrenderer.h"

FtRenderer::FtRenderer()
    : IGlyphRenderer ()
    , m_ftLibrary(0)
    , m_ftFace(0)
{}

FtRenderer::~FtRenderer()
{}

bool FtRenderer::renderGlyph(const QColor &color, const QColor &bgColor, const QSize &targetSize)
{
    return true;
}