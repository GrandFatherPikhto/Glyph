#ifndef IGLYPHRENDERERFACTORY_H
#define IGLYPHRENDERERFACTORY_H

#include "iglyphrenderer.h"

class IGlyphRendererFactory
{
public:
    static std::unique_ptr<IGlyphRenderer> createFtRenderer();
};

#endif // IGLYPHRENDERERFACTORY_H
