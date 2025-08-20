#include "iglyphrendererfactory.h"
#include "ftrenderer.h"

std::unique_ptr<IGlyphRenderer> IGlyphRendererFactory::createFtRenderer() {
    return std::make_unique<FtRenderer>();
}
