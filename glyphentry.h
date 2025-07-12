#ifndef GLYPHENTRY_H
#define GLYPHENTRY_H

#include <QVector>
#include <QHash>
#include <QString>
#include <QRect>
#include <QDataStream>
#include <QDebug>

class GlyphEntry {
public:
    GlyphEntry(int newGlyphSize, const QRect &newGlyphRect, const QVector<bool> &newGlyphPixels, const QString &newFontPath)
        : m_glyphPixels(newGlyphPixels)
        , m_glyphSize(newGlyphSize)
        , m_glyphRect(newGlyphRect)
        , m_glyphCols(newGlyphRect.width())
        , m_glyphRows(newGlyphRect.height())
        , m_fontPath(newFontPath)
        , m_offset(QPoint(0,0))
    {}

    GlyphEntry()
        : m_glyphSize(-1)
        , m_glyphRect(QRect())
        , m_glyphCols(-1)
        , m_glyphRows(-1)
        , m_fontPath("")
    {
        m_glyphPixels.clear();
    }

    GlyphEntry(const GlyphEntry & newEntry)
        : m_glyphCols(newEntry.m_glyphCols)
        , m_glyphRows(newEntry.m_glyphRows)
        , m_glyphSize(newEntry.m_glyphSize)
        , m_glyphRect(newEntry.m_glyphRect)
        , m_glyphPixels(newEntry.m_glyphPixels)
        , m_fontPath(newEntry.m_fontPath)
    {

    }

    void setFontPath (const QString &newFontPath) {
        m_fontPath = newFontPath;
        qDebug() << __FUNCTION__ << m_fontPath;
    }

    const QString & fontPath() const {
        return m_fontPath;
    }

    void setOffset (const QPoint &newOffset)
    {
        m_offset = newOffset;
    }

    const QPoint & offset() { return m_offset; }

    void setGlyphPixels(const QVector<bool> &newGlyphPixels) {
        m_glyphPixels = newGlyphPixels;
    }
    const QVector<bool> & glyphPixels () { return m_glyphPixels; }

    bool glyphPixelsIsValid () {
        return (m_glyphRows * m_glyphCols == m_glyphPixels.size());
    }

    void setGlyphRect(const QRect &newGlyphRect) {
        m_glyphRect = newGlyphRect;
        m_glyphCols = m_glyphRect.width();
        m_glyphRows = m_glyphRect.height();
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
        if (y > m_glyphRows)
            return false;
        if (x > m_glyphCols)
            return false;
        int pos = y * m_glyphCols + x;
        if (pos >= m_glyphPixels.size())
            return false;

        return m_glyphPixels.at(pos) ? 1 : 0;
    }

    void printValidation() {
        QString validation((m_glyphSize >= 6 && !m_fontPath.isEmpty()) ? "Valid" : "No Valid");
        qDebug() << "GlyphEntry: " << validation << "GlyphRect: " << m_glyphRect << ", Glyph Size: " << m_glyphSize << ", Glyph Rows: " << m_glyphRows << ", Glyph Size: " << m_glyphCols << ", Glyph Pixels Size: " << m_glyphPixels.size() << ", Font Path: " << m_fontPath;
    }

    bool isValid () {
        // && m_glyphRect != QRect() && m_glyphRows * m_glyphCols == m_glyphPixels.size()
        return (m_glyphSize >= 6 && !m_fontPath.isEmpty());
    }

    friend QDataStream &operator<<(QDataStream &out, const GlyphEntry &entry) {
        out << entry.m_glyphSize << entry.m_glyphRect;
        // Сериализация QVector<bool> (конвертируем в QByteArray)
        QByteArray pixelsBytes;
        for (bool bit : entry.m_glyphPixels) {
            pixelsBytes.append(bit ? 1 : 0);
        }
        out << pixelsBytes;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GlyphEntry &entry) {
        QByteArray pixelsBytes;
        in >> entry.m_glyphSize >> entry.m_glyphRect >> pixelsBytes;
        // Десериализация QVector<bool>
        entry.m_glyphPixels.clear();
        for (char byte : pixelsBytes) {
            entry.m_glyphPixels.append(byte != 0);
        }
        return in;
    }


private:
        int m_glyphSize;
        QRect m_glyphRect;
        QVector<bool> m_glyphPixels;
        int m_glyphCols;
        int m_glyphRows;
        QPoint m_offset;
        QString m_fontPath;
};

#endif // GLYPHENTRY_H
