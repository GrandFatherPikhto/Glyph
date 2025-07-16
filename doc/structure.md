@startuml
skinparam defaultFontName Arial

' --- Наследники QObject ---
class GlyphWidget <<QObject>> {
    +void setGlyph(const QSharedPointer<Glyph>& glyph)
    +signals:
    +    void userLayerChanged()
    +slots:
    +    void onUserLayerModified()
}

class Glyph <<QObject>> {
    +QString symbol
    +QString fontPath
    +QSize gridSize
    +QRect glyphRect
    +int baseline
    +QSharedPointer<GlyphImage> templateImage
    +QSharedPointer<UserGlyphLayer> userLayer

    +QSharedPointer<GlyphImage> getFinalImage()
    +void resetUserLayerFromTemplate()

    +signals:
    +    void userLayerUpdated()
}

class GlyphImage <<QObject>> {
    +QImage gridImage
    +QImage previewImage

    +void clear()
    +void applyFilter()
}

class UserGlyphLayer <<QObject>> {
    +explicit UserGlyphLayer(const QSize& size)

    +void setPixel(int x, int y, bool on)
    +bool pixel(int x, int y) const
    +void clear()
    +void copyFrom(const QImage& templateImg)
    +QImage toImage() const
    +QByteArray toMCUCode() const

    +signals:
    +    void layerChanged()
}

class GlyphManager <<QObject>> {
    -QHash<GlyphKey, Glyph> glyphMetadata
    -QHash<GlyphKey, QSharedPointer<GlyphImage>> glyphImages

    -FreeTypeRenderer ftRenderer
    -ImageTemplateRenderer imgRenderer

    +QSharedPointer<GlyphImage> getGlyph(const GlyphKey &key)

    +signals:
    +    void glyphUpdated(const GlyphKey &key)
}

class GlyphRenderer {
    +static QSharedPointer<GlyphImage> render(const Glyph &glyph, SourceType source)
}

class FreeTypeRenderer
class ImageTemplateRenderer

enum SourceType {
    FONT
    TEMPLATE
}

' --- Отношения ---
GlyphWidget --> Glyph : uses
Glyph --> GlyphImage : has templateImage
Glyph --> UserGlyphLayer : has userLayer
UserGlyphLayer ..|> QObject
GlyphImage ..|> QObject
Glyph ..|> QObject
GlyphWidget ..|> QObject
GlyphManager ..|> QObject

GlyphManager --> Glyph : manages
GlyphManager --> GlyphImage : caches
GlyphManager --> FreeTypeRenderer
GlyphManager --> ImageTemplateRenderer
GlyphRenderer ..> SourceType
GlyphRenderer ..> GlyphImage

' --- Сигналы и слоты ---
GlyphWidget : userLayerChanged()
GlyphWidget : onUserLayerModified()

UserGlyphLayer : layerChanged()
Glyph : userLayerUpdated()
GlyphManager : glyphUpdated(key)

@enduml
