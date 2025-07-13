#ifndef GLYPHENTRY_H
#define GLYPHENTRY_H

#include <QVector>
#include <QHash>
#include <QString>
#include <QRect>
#include <QDataStream>
#include <QDebug>
#include <QFont>
#include <QSharedPointer>

#include "glyphkey.h"

class Glyph {
public:
    Glyph(const QChar &newCharacter = QChar(), int newGridSize = -1, const QFont &newFont = QFont(), const QString &newFontPath = QString(), int newGlyphSize = -1)
        : m_font(newFont)
        , m_fontPath(newFontPath)
        , m_glyphSize(newGridSize)
        , m_gridSize(newGridSize)
        , m_character(newCharacter)
        , m_offset(QPoint(0,0))
        , m_glyphRect(QRect())
    {
        m_glyphPixels.clear();
    }

    Glyph(const Glyph & glyph)
        : m_font(glyph.m_font)
        , m_fontPath(glyph.m_fontPath)
        , m_glyphSize(glyph.m_glyphSize)
        , m_gridSize(glyph.m_gridSize)
        , m_character(glyph.m_character)
        , m_offset(glyph.m_offset)
        , m_glyphRect(glyph.m_glyphRect)
        , m_glyphPixels(glyph.m_glyphPixels)
    {

    }

    Glyph(QSharedPointer<Glyph> glyph)
        : m_font(glyph->m_font)
        , m_fontPath(glyph->m_fontPath)
        , m_glyphSize(glyph->m_glyphSize)
        , m_gridSize(glyph->m_gridSize)
        , m_character(glyph->m_character)
        , m_offset(glyph->m_offset)
        , m_glyphRect(glyph->m_glyphRect)
        , m_glyphPixels(glyph->m_glyphPixels)
    {

    }

    // Фонт
    void setFont(const QFont &newFont) { m_font = newFont; }
    const QFont& font() const { return m_font; }

    // Символ
    void setCharacter(const QChar &newCharacter) {m_character = newCharacter;}
    const QChar& character() const { return m_character; }

    // Размер сетки
    void setGridSize(int newGridSize) { m_gridSize = newGridSize; }
    int gridSize() const { return m_gridSize; }


    // Путь к файлу шрифта. Нужен для FreeType
    void setFontPath (const QString &newFontPath) {
        m_fontPath = newFontPath;
        qDebug() << __FUNCTION__ << m_fontPath;
    }
    const QString & fontPath() const {
        return m_fontPath;
    }

    // Смещение m_glyphRect для отрисовки (чтобы попадал в сетку m_gridSize x m_gridSize
    void setOffset (const QPoint &newOffset)
    {
        m_offset = newOffset;
    }

    const QPoint & offset() { return m_offset; }

    void moveLeft() {
        m_offset -= QPoint(1, 0);
    }

    void moveTop() {
        // m_glyphRect.translate(0, -1);
        m_offset -= QPoint(0, 1);
    }

    void moveDown() {
        // m_glyphRect.translate(0, 1);
        m_offset += QPoint(0, 1);
    }

    void moveRight() {
        // m_glyphRect.translate(1, 0);
        m_offset += QPoint(1, 0);
    }

    void moveCenter() {

    }

    void setGlyphPixels(const QVector<bool> &newGlyphPixels) {
        m_glyphPixels = newGlyphPixels;
    }
    const QVector<bool> & glyphPixels () { return m_glyphPixels; }

    bool glyphPixelsIsValid () {
        return (m_glyphRect != QRect() && m_glyphPixels.size() && m_glyphRect.width() * m_glyphRect.height() == m_glyphPixels.size());
    }

    void setGlyphRect(const QRect &newGlyphRect) {
        m_glyphRect = newGlyphRect;
    }

    QRect glyphRect() {
        return m_glyphRect.translated(m_offset);
    }

    int glyphRows() {
        return m_glyphRect.height();
    }

    int glyphCols() {
        return m_glyphRect.width();
    }

    int glyphLeft() {
        return m_glyphRect.translated(m_offset).left();
    }

    int glyphTop () {
        return m_glyphRect.translated(m_offset).top();
    }

    void setGlyphSize(int newGlyphSize) {
        m_glyphSize = newGlyphSize;
    }

    int glyphSize() { return m_glyphSize; }

    void resetGlyphPixels () { m_glyphPixels.clear(); }

    int addPixel(bool color) {
        m_glyphPixels.append(color);
        return m_glyphPixels.size();
    }

    bool getPixel(int x, int y) {
        if (y > m_glyphRect.height())
            return false;
        if (x > m_glyphRect.width())
            return false;
        int pos = y * m_glyphRect.width() + x;
        if (pos >= m_glyphPixels.size())
            return false;

        return m_glyphPixels.at(pos) ? 1 : 0;
    }

    QString toString() {
        QString validation((isValid() ? "Valid" : "No Valid"));
        QString glyphRect = QString("(%1, %2, (%3 x %4))").arg(m_glyphRect.left()).arg(m_glyphRect.top()).arg(m_glyphRect.width()).arg(m_glyphRect.height());
        QString glyphOffset = QString("(%1, %2)").arg(m_offset.x()).arg(m_offset.y());
        return QString("GlyphEntry: %1, Font: %2, FontPath: %3, Character: %4, Glyph Size: %5, GlyphRect: %6, Offset: %7, Grid Size: %8, Pixels: %9")
            .arg(validation)
            .arg(m_font.family())
            .arg(m_fontPath)
            .arg(m_character)
            .arg(m_glyphSize)
            .arg(glyphRect)
            .arg(glyphOffset)
            .arg(m_gridSize)
            .arg(m_glyphPixels.size());
    }

    bool isValid () {
        // m_font != QFont() && m_character != QChar() && m_gridSize > 0 && m_glyphSize >= 6 && !m_fontPath.isEmpty()
        return (m_glyphSize >= 6 && !m_fontPath.isEmpty());
    }

    friend QDataStream &operator<<(QDataStream &out, const Glyph &glyph) {
        // out << entry.m_glyphSize << entry.m_glyphRect;
        out << glyph.m_font << glyph.m_fontPath << glyph.m_character << glyph.m_glyphSize << glyph.m_gridSize << glyph.m_glyphRect << glyph.m_offset;
        // Сериализация QVector<bool> (конвертируем в QByteArray)
        QByteArray pixelsBytes;
        for (bool bit : glyph.m_glyphPixels) {
            pixelsBytes.append(bit ? 1 : 0);
        }
        out << pixelsBytes;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Glyph &glyph) {
        QByteArray pixelsBytes;
        in >> glyph.m_font >> glyph.m_fontPath >> glyph.m_character >> glyph.m_glyphSize >> glyph.m_gridSize >> glyph.m_glyphRect >> glyph.m_offset >> pixelsBytes;
        // Десериализация QVector<bool>
        glyph.m_glyphPixels.clear();
        for (char byte : pixelsBytes) {
            glyph.m_glyphPixels.append(byte != 0);
        }
        return in;
    }

    GlyphKey glyphKey() {
        return GlyphKey(m_character.unicode(), m_gridSize);
    }

private:

    QFont m_font; //< Исходны шрифт для рендеринга глифа
    QString m_fontPath; //< Путь к файлу шрифта. Нужен для FreeType
    int m_glyphSize; //< Размер глифа в пикселях
    int m_gridSize; //< Раземр пиксельной сетки, в которой должен располагаться глиф
    QChar m_character; //< Символ
    QRect m_glyphRect; //< Пространство и положение (Left/Top), которое занимает глиф
    QVector<bool> m_glyphPixels; //< Вектор пикселей глифа
    QPoint m_offset; //< Смещение глифа относительно m_glyphRect
};

inline uint qHash(const Glyph &entry, uint seed) {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, qHash(entry.font().family()));
    seed = hash(seed, entry.character().unicode());
    seed = hash(seed, entry.gridSize());
    return seed;
}

#endif // GLYPHENTRY_H
