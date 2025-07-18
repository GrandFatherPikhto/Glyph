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
    // Первые два параметры ДОЛЖНЫ БЫТЬ заполнены. Потому, что это параметры GlyphKey
    GlyphMeta(const QChar &newCharacter, int bitmapDimension, int glyphSize = -1, const QFont &newFont = QFont(), const QString &newFontPath = QString())
        : m_character(newCharacter)
        , m_glyphSize(glyphSize)
        , m_bitmapDimension(bitmapDimension)
        , m_font(newFont)
        , m_fontPath(newFontPath)
        , m_offset(QPoint(0,0))
        , m_glyphRect(QRect())
        , m_previewRect(QRect())
        , m_dirty(false)
        , m_resized(false)
    {

    }

    GlyphMeta(const GlyphMeta & glyphMeta)
        : m_character(glyphMeta.m_character)
        , m_glyphSize(glyphMeta.m_glyphSize)
        , m_bitmapDimension(glyphMeta.m_bitmapDimension)
        , m_font(glyphMeta.m_font)
        , m_fontPath(glyphMeta.m_fontPath)
        , m_offset(glyphMeta.m_offset)
        , m_glyphRect(glyphMeta.m_glyphRect)
        , m_previewRect(glyphMeta.m_previewRect)
        , m_dirty(false)
        , m_resized(glyphMeta.m_resized)
    {
        
    }

    GlyphMeta(QSharedPointer<GlyphMeta> glyphMeta)
        : m_character(glyphMeta->m_character)
        , m_glyphSize(glyphMeta->m_glyphSize)
        , m_bitmapDimension(glyphMeta->m_bitmapDimension)
        , m_font(glyphMeta->m_font)
        , m_fontPath(glyphMeta->m_fontPath)
        , m_offset(glyphMeta->m_offset)
        , m_glyphRect(glyphMeta->m_glyphRect)
        , m_previewRect(glyphMeta->m_previewRect)
        , m_dirty(glyphMeta->m_dirty)
        , m_resized(glyphMeta->m_resized)
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
            && (m_dirty == glyphMeta->m_dirty)
            && (m_resized == glyphMeta->m_resized)
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
            && (m_dirty == glyphMeta.m_dirty)
            && (m_resized == glyphMeta.m_resized)
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
            || (m_dirty != glyphMeta->m_dirty)
            || (m_resized == glyphMeta->m_resized)
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
            || (m_dirty != glyphMeta.m_dirty)
            || (m_resized == glyphMeta.m_resized)
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

    // Смещение m_glyphRect для отрисовки (чтобы попадал в сетку m_bitmapDimensions x m_bitmapDimensions
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

    void moveCenter() {

    }

    void setGlyphRect(const QRect &newGlyphRect) {
        m_glyphRect = newGlyphRect;
    }

    QRect glyphRect() {
        // return m_glyphRect.translated(m_offset);
        return m_glyphRect;
    }

    void setPreviewRect(const QRect &newPreviewRect)
    {
        m_previewRect = newPreviewRect;
    }

    QRect previewRect ()
    {
        return m_previewRect;
    }

    int glyphRows() {
        return m_glyphRect.height();
    }

    int glyphCols() {
        return m_glyphRect.width();
    }

    int glyphLeft() {
        // return m_glyphRect.translated(m_offset).left();
        return m_glyphRect.left();
    }

    int glyphTop () {
        // return m_glyphRect.translated(m_offset).top();
        return m_glyphRect.top();
    }

    void setGlyphSize(int newGlyphSize) {
        m_glyphSize = newGlyphSize;
    }

    int glyphSize() { return m_glyphSize; }

    void setDirty() { m_dirty = true; }
    void resetDirty() { m_dirty = false; }
    bool dirty() { return m_dirty; }

    void setResized () { m_resized = true; }
    void resetResized () { m_resized = false; }
    bool resized () { return m_resized; }

    QString toString() {
        QString validation((isValid() ? "Valid" : "No Valid"));
        QString glyphRect = QString("(%1, %2, (%3 x %4))").arg(m_glyphRect.left()).arg(m_glyphRect.top()).arg(m_glyphRect.width()).arg(m_glyphRect.height());
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
        // m_font != QFont() && m_character != QChar() && m_bitmapDimensions > 0 && m_glyphSize >= 6 && !m_fontPath.isEmpty()
        return (m_character != QChar() && m_glyphSize >= 6 && !m_fontPath.isEmpty());
    }

    friend QDataStream &operator<<(QDataStream &out, const GlyphMeta &glyph) {
// qDebug() << "operator <<";
        // out << entry.m_glyphSize << entry.m_glyphRect;
        out << glyph.m_font << glyph.m_fontPath << glyph.m_character << glyph.m_glyphSize << glyph.m_bitmapDimension << glyph.m_glyphRect << glyph.m_offset;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GlyphMeta &glyph) {
        in >> glyph.m_font >> glyph.m_fontPath >> glyph.m_character >> glyph.m_glyphSize >> glyph.m_bitmapDimension >> glyph.m_glyphRect >> glyph.m_offset;
        return in;
    }

    GlyphKey glyphKey() {
        return GlyphKey(m_character.unicode(), m_bitmapDimension, m_font.family());
    }

    QRect paintRect() {
        QRect glyphRect(m_glyphRect);
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
    // int m_gridDimension; //< Размер пиксельной сетки, в которой размещён глиф. Может быть больше bitmapDimension
    QChar m_character; //< Символ
    QRect m_glyphRect; //< Пространство и положение (Left/Top), которое занимает глиф
    QRect m_previewRect; //< Пространство и положение, которое занимает preview
    QPoint m_offset; //< Смещение глифа относительно m_glyphRect
    bool m_dirty; //< Запрос на перерисовку
    bool m_resized; //< переразмеривание
};

inline uint qHash(const GlyphMeta &entry, uint seed) {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, entry.character().unicode());
    seed = hash(seed, entry.bitmapDimension());
    return seed;
}

#endif // GLYPHENTRY_H
