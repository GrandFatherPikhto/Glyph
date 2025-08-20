#ifndef __IGLYPHRENDER_H__
#define __IGLYPHRENDER_H__

#include "fontcontext.h"
#include "imagecontext.h"

class IGlyphRenderer {
public:

    virtual ~IGlyphRenderer() = default;
    virtual bool renderGlyph(const FontContext &font, QSharedPointer<ImageContext> image) = 0;
    
    virtual QString rendererName() const = 0;

protected:
    // Конструктор protected - нельзя создать экземпляр абстрактного класса
    explicit IGlyphRenderer() {}
};

#endif // __IGLYPHRENDER_H__
