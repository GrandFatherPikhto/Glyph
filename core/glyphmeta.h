#ifndef GLYPHENTRY_H
#define GLYPHENTRY_H

#include <QChar>
#include <QString>
#include <QRect>
#include <QDataStream>
#include <QDebug>
#include <QFont>
#include <QSharedPointer>

#include "glyphkey.h"

class GlyphMeta {

public:

    enum Type {
        ImageUser,
        ImageTemplate,
        ImageDraw,
        ImagePreview
    };

    // Первые два параметры ДОЛЖНЫ БЫТЬ заполнены. Потому, что это параметры GlyphKey
    GlyphMeta(const QChar &newCharacter, int bitmapDimension, int glyphSize, const QFont &newFont = QFont(), const QString &newFontPath = QString(), bool temporary = false, bool dirty = false)
        : m_character(newCharacter)
        , m_glyphSize(glyphSize)
        , m_bitmapDimension(bitmapDimension)
        , m_font(newFont)
        , m_fontPath(newFontPath)
        , m_offset(QPoint(0,0))
        , m_templateRect(QRect())
        , m_previewRect(QRect())
        , m_drawRect(QRect())
        , m_userRect(QRect())
        , m_dirty(false)
        , m_resized(false)
        , m_temporary(temporary)
        , m_layerDraw(QSharedPointer<QImage>())
        , m_layerPreview(QSharedPointer<QImage>())
        , m_layerTemplate(QSharedPointer<QImage>())
        , m_previewSize(QSize())
    {

    }

    GlyphMeta(const GlyphMeta & glyphMeta)
        : m_character(glyphMeta.m_character)
        , m_glyphSize(glyphMeta.m_glyphSize)
        , m_bitmapDimension(glyphMeta.m_bitmapDimension)
        , m_font(glyphMeta.m_font)
        , m_fontPath(glyphMeta.m_fontPath)
        , m_offset(glyphMeta.m_offset)
        , m_templateRect(glyphMeta.m_templateRect)
        , m_previewRect(glyphMeta.m_previewRect)
        , m_dirty(glyphMeta.m_dirty)
        , m_resized(glyphMeta.m_resized)
        , m_temporary(glyphMeta.m_temporary)
        , m_layerDraw(glyphMeta.m_layerDraw)
        , m_layerPreview(glyphMeta.m_layerPreview)
        , m_layerTemplate(glyphMeta.m_layerTemplate)
        , m_previewSize(glyphMeta.m_previewSize)
    {
        
    }

    GlyphMeta(QSharedPointer<GlyphMeta> glyphMeta)
        : m_character(glyphMeta->m_character)
        , m_glyphSize(glyphMeta->m_glyphSize)
        , m_bitmapDimension(glyphMeta->m_bitmapDimension)
        , m_font(glyphMeta->m_font)
        , m_fontPath(glyphMeta->m_fontPath)
        , m_offset(glyphMeta->m_offset)
        , m_templateRect(glyphMeta->m_templateRect)
        , m_previewRect(glyphMeta->m_previewRect)
        , m_dirty(glyphMeta->m_dirty)
        , m_resized(glyphMeta->m_resized)
        , m_temporary(glyphMeta->m_temporary)
        , m_layerDraw(glyphMeta->m_layerDraw)
        , m_layerPreview(glyphMeta->m_layerPreview)
        , m_layerTemplate(glyphMeta->m_layerTemplate)
        , m_previewSize(glyphMeta->m_previewSize)
    {

    }

    bool operator == (QSharedPointer<GlyphMeta> glyphMeta)
    {
        return (
            (m_font == glyphMeta->m_font)
            && (m_fontPath == glyphMeta->m_fontPath)
            && (m_glyphSize == glyphMeta->m_glyphSize)
            && (m_bitmapDimension == glyphMeta->m_bitmapDimension)
            && (m_character == glyphMeta->m_character)
            && (m_offset == glyphMeta->m_offset)
            // && (m_dirty == glyphMeta->m_dirty)
            // && (m_resized == glyphMeta->m_resized)
            // && (m_temporary == glyphMeta->m_temporary)
        );
    }

    bool operator == (const GlyphMeta &glyphMeta)
    {
        return (
            (m_font == glyphMeta.m_font)
            && (m_fontPath == glyphMeta.m_fontPath)
            && (m_glyphSize == glyphMeta.m_glyphSize)
            && (m_bitmapDimension == glyphMeta.m_bitmapDimension)
            && (m_character == glyphMeta.m_character)
            && (m_offset == glyphMeta.m_offset)
            // && (m_dirty == glyphMeta.m_dirty)
            // && (m_temporary == glyphMeta.m_temporary)
        );
    }

    bool operator != (QSharedPointer<GlyphMeta> glyphMeta)
    {
        return (
               (m_font != glyphMeta->m_font)
            || (m_fontPath != glyphMeta->m_fontPath)
            || (m_glyphSize != glyphMeta->m_glyphSize)
            || (m_bitmapDimension != glyphMeta->m_bitmapDimension)
            || (m_character != glyphMeta->m_character)
            || (m_offset != glyphMeta->m_offset)
            // || (m_dirty != glyphMeta->m_dirty)
            // || (m_temporary != glyphMeta->m_temporary)
        );
    }

    bool operator != (const GlyphMeta &glyphMeta)
    {
        return (
               (m_font != glyphMeta.m_font)
            || (m_fontPath != glyphMeta.m_fontPath)
            || (m_glyphSize != glyphMeta.m_glyphSize)
            || (m_bitmapDimension != glyphMeta.m_bitmapDimension)
            || (m_character != glyphMeta.m_character)
            || (m_offset != glyphMeta.m_offset)
            // || (m_dirty != glyphMeta.m_dirty)
            // || (m_temporary != glyphMeta.m_temporary)
        );
    }


    // Фонт
    void setFont(const QFont &newFont) { m_font = newFont; }
    const QFont& font() const { return m_font; }

    // Символ
    void setCharacter(const QChar &newCharacter) {m_character = newCharacter;}
    const QChar& character() const { return m_character; }

    // Размер битмапа
    void setBitmapDimension(int newGridSize) { m_bitmapDimension = newGridSize; }
    int bitmapDimension() const { return m_bitmapDimension; }

    // Путь к файлу шрифта. Нужен для FreeType
    void setFontPath (const QString &newFontPath) {
        m_fontPath = newFontPath;
    }
    const QString & fontPath() const {
        return m_fontPath;
    }

    // Смещение m_templateRect для отрисовки (чтобы попадал в сетку m_bitmapDimensions x m_bitmapDimensions
    void setOffset (const QPoint &newOffset)
    {
        m_offset = newOffset;
    }

    const QPoint & offset() { return m_offset; }

    void moveLeft() {
        m_offset += QPoint(1, 0);
    }

    void moveTop() {
        m_offset -= QPoint(0, 1);
    }

    void moveDown() {
        m_offset += QPoint(0, 1);
    }

    void moveRight() {
        m_offset -= QPoint(1, 0);
    }

    void resetOffset() {

    }

    void setTemplateRect(const QRect &newGlyphRect) {
        m_templateRect = newGlyphRect;
    }

    QRect templateRect() {
        // return m_templateRect.translated(m_offset);
        return m_templateRect;
    }

    QSize templateSize(int factor = 1) {
        return m_templateRect.size() * factor;
    }

    void setPreviewRect(const QRect &newPreviewRect)
    {
        m_previewRect = newPreviewRect;
    }

    QRect previewRect ()
    {
        return m_previewRect;
    }

    void setDrawRect(const QRect &rect)
    {
        m_drawRect = rect;
    }

    QRect drawRect ()
    {
        return m_drawRect;
    }

    void setUserRect(const QRect &rect)
    {
        m_userRect = rect;
    }

    QRect userRect()
    {
        return m_userRect;
    }

    int glyphRows() {
        return m_templateRect.height();
    }

    int glyphCols() {
        return m_templateRect.width();
    }

    int glyphLeft() {
        // return m_templateRect.translated(m_offset).left();
        return m_templateRect.left();
    }

    int glyphTop () {
        // return m_templateRect.translated(m_offset).top();
        return m_templateRect.top();
    }

    void setGlyphSize(int newGlyphSize) {
        m_glyphSize = newGlyphSize;
    }

    int glyphSize() { return m_glyphSize; }

    void setLayerTemplate(QSharedPointer<QImage> image)
    {
        m_layerTemplate = image;
    }

    QSharedPointer<QImage> layerTemplate()
    {
        return m_layerTemplate;
    }

    void setLayerPreview (QSharedPointer<QImage> image)
    {
        m_layerPreview = image;
    }

    QSharedPointer<QImage> layerPreview()
    {
        return m_layerPreview;
    }

    QSharedPointer<QImage> layerDraw ()
    {
        return m_layerDraw;
    }

    void setLayerDraw (QSharedPointer<QImage> image)
    {
        m_layerDraw = image;
    }

    const QSize &previewSize()
    {
        return m_previewSize;
    }

    void setPreviewSize(const QSize &size)
    {
        m_previewSize = size;
    }

    void setDirty() { m_dirty = true; }
    void resetDirty() { m_dirty = false; }
    bool isDirty() { return m_dirty; }

    void setTemporary() { m_temporary = true; }
    void resetTemporary() { m_temporary = false; }
    bool isTemporary() { return m_temporary; }

    void setResized() { m_resized = true; }
    void resetResized() { m_resized = false; }
    bool isResized() { return m_resized; }

    QString toString() {
        QString validation((isValid() ? "Valid" : "No Valid"));
        QString glyphRect = QString("(%1, %2, (%3 x %4))").arg(m_templateRect.left()).arg(m_templateRect.top()).arg(m_templateRect.width()).arg(m_templateRect.height());
        QString glyphOffset = QString("(%1, %2)").arg(m_offset.x()).arg(m_offset.y());
        return QString("GlyphEntry: %1, Character: %2, Glyph Size: %3, Bitmap Dimension: %4, Font: %5, Font Path: %6, Glyph Rect: %7, Offset: %8, Dirty: %9")
            .arg(validation)
            .arg(m_character)
            .arg(m_glyphSize)
            .arg(m_bitmapDimension)
            .arg(m_font.family())
            .arg(m_fontPath)
            .arg(glyphRect)
            .arg(glyphOffset)
            .arg(m_dirty);
    }

    bool isValid () {
        return (m_character != QChar() && m_glyphSize >= 6 && m_bitmapDimension >= 6);
    }

    friend QDataStream &operator<<(QDataStream &out, const GlyphMeta &glyph) {
// qDebug() << "operator <<";
        // out << entry.m_glyphSize << entry.m_templateRect;
        out << glyph.m_font << glyph.m_fontPath << glyph.m_character << glyph.m_glyphSize << glyph.m_bitmapDimension << glyph.m_templateRect << glyph.m_offset;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GlyphMeta &glyph) {
        in >> glyph.m_font >> glyph.m_fontPath >> glyph.m_character >> glyph.m_glyphSize >> glyph.m_bitmapDimension >> glyph.m_templateRect >> glyph.m_offset;
        return in;
    }

    GlyphKey key() {
        return GlyphKey(m_character.unicode(), m_bitmapDimension, m_font.family());
    }

    QRect paintRect() {
        QRect glyphRect(m_templateRect);
        if (glyphRect.top() < glyphRect.height())
        {
            glyphRect.translate(0, glyphRect.height() - glyphRect.top());
        }

        glyphRect.translate(QPoint(-glyphRect.left(), -glyphRect.top() + (m_bitmapDimension - glyphRect.top())));
        //qDebug() << glyphRect;
        return glyphRect;
    }

private:
    QFont m_font; //< Исходный шрифт для рендеринга глифа
    QString m_fontPath; //< Путь к файлу шрифта. Нужен для FreeType
    int m_glyphSize; //< Размер глифа в пикселях
    int m_bitmapDimension; //< Раземр сетки, в которой располагатеся глиф
    QChar m_character; //< Символ
    QRect m_templateRect; //< Пространство и положение (Left/Top), которое занимает глиф
    QRect m_previewRect; //< Пространство и положение, которое занимает preview
    QRect m_drawRect; //< Пространство пользовательского глифа
    QRect m_userRect; //<< 
    QPoint m_offset; //< Смещение глифа относительно m_templateRect
    bool m_dirty; //< Запрос на перерисовку
    bool m_temporary; //< Добавлять/Не добавлять в базу данных.
    bool m_resized; //< Обновлён размер предпросмотрового изображения глифа

    QSharedPointer<QImage> m_layerTemplate;
    QSharedPointer<QImage> m_layerPreview;
    QSharedPointer<QImage> m_layerDraw;

    QSize m_previewSize;
};

inline uint qHash(const GlyphMeta &entry, uint seed) {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, entry.character().unicode());
    seed = hash(seed, entry.bitmapDimension());
    return seed;
}

#endif // GLYPHENTRY_H
