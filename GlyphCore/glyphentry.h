#ifndef __GLYPHENTRY_H__
#define __GLYPHENTRY_H__

#include <QSharedPointer>
#include <QChar>
#include <QFont>

#include "glyphkey.h"
#include "bitmapdimension.h"


class GlyphEntry {
public:
    GlyphEntry(QSharedPointer<BitmapDimension> bitmapDimension, const QChar &character, int glyphSize, bool temporary = true, const QFont &font = QFont(), const QString &fontPath = QString())
        : m_bitmapDimension(bitmapDimension)
        , m_character(character)
        , m_glyphSize(glyphSize)
        , m_glyphFont(font)
    {
        m_key = GlyphKey(m_character, m_bitmapDimension->bitmapDimension());
    }

    void setTemporary (bool value) { m_temporary = value; }
    bool temporary () { return m_temporary; }
    const QString &fontPath () { return m_fontPath; }
    const QFont & glyphFont() { return m_glyphFont; }

    const QChar & character() { return m_character; }
    int glyphSize () { return m_glyphSize; }

    GlyphKey & glyphKey() { return m_key; }

    QSharedPointer<BitmapDimension> bitmapDimension () { return m_bitmapDimension; }

private:
    int m_glyphSize;
    bool m_temporary;

    QChar m_character;
    QFont m_glyphFont;
    QString m_fontPath;

    GlyphKey m_key;

    QSharedPointer<BitmapDimension> m_bitmapDimension;
};

#endif // __GLYPHENTRY_H__