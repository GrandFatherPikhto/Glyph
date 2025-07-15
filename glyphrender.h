#ifndef __GLYPHRENDERER_H__
#define __GLYPHRENDERER_H__

#include <QSharedPointer>

#include "glyphmeta.h"

class GlyphRender {
public:
    enum ImageType {
        Original,
        UserModified,
        Template,
        Preview
    };

    // Основной метод рендеринга
    static QSharedPointer<QImage> renderGlyph(
        const GlyphMeta& meta,
        ImageType type,
        const QSize& targetSize = QSize(),
        Qt::TransformationMode mode = Qt::FastTransformation
    ) {
        switch(type) {
            case Original:
                return renderFromFont(meta, targetSize, mode);
            case UserModified:
                return applyUserModifications(meta, targetSize, mode);
            case Template:
                return renderTemplate(meta, targetSize, mode);
            case Preview:
                return generatePreview(meta, targetSize, mode);
        }
        return nullptr;
    }

private:
    // Рендеринг из шрифта через FreeType
    static QSharedPointer<QImage> renderFromFont(
        const GlyphMeta& meta,
        const QSize& targetSize,
        Qt::TransformationMode mode
    ) {
        FT_Face face = loadFontFace(meta.fontPath);
        FT_Set_Pixel_Sizes(face, 0, meta.baseSize.height());
        
        if (FT_Load_Char(face, meta.character.unicode(), FT_LOAD_RENDER)) {
            return nullptr;
        }

        QImage img = convertFTBitmap(&face->glyph->bitmap);
        if (!targetSize.isEmpty()) {
            img = img.scaled(targetSize, Qt::KeepAspectRatio, mode);
        }
        
        return QSharedPointer<QImage>::create(img);
    }

    // Обработка пользовательских модификаций
    static QSharedPointer<QImage> applyUserModifications(
        const GlyphMeta& meta,
        const QSize& targetSize,
        Qt::TransformationMode mode
    ) {
        // ... логика применения пользовательских правок
    }

    // Загрузка шаблонных изображений
    static QSharedPointer<QImage> renderTemplate(
        const GlyphMeta& meta,
        const QSize& targetSize,
        Qt::TransformationMode mode
    ) {
        if (meta.templatePath.isEmpty()) return nullptr;
        
        QImage img(meta.templatePath);
        if (!targetSize.isEmpty()) {
            img = img.scaled(targetSize, Qt::KeepAspectRatio, mode);
        }
        
        return QSharedPointer<QImage>::create(img);
    }
};

#endif // __GLYPHRENDERER_H__