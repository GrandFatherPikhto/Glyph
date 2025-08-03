#ifndef GLYPHPROFILE_H
#define GLYPHPROFILE_H

#include <QObject>
#include <QFont>
#include <QString>
#include <QSharedPointer>
#include <QDataStream>
#include <QDebug>

#include "gridpaddings.h"
#include "profilekey.h"

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GlyphProfile
{
public:
    explicit GlyphProfile(const ProfileKey &key, int glyphSize = -1, bool fixedFont = false, const QFont &font = QFont(), const QString &fontPath = QString())
        : m_key(key)
        , m_glyphSize(glyphSize)
        , m_fixedFont(fixedFont)
        , m_font(font)
        , m_fontPath(fontPath)
        , m_gridPaddings(GridPaddings())
    {}

    ~GlyphProfile(){}

    void setFont(const QFont &newFont) { m_font = newFont; }
    const QFont & font() const { return m_font; }

    void setFontPath(const QString &newPath) { m_fontPath = newPath; }
    const QString & fontPath () { return m_fontPath; }

    const QString & name() const { return m_key.name(); }

    void setGlyphSize (int newSize ) { m_glyphSize = newSize; }
    int glyphSize() { return m_glyphSize; }

    void setFixedFont() { m_fixedFont = true; }
    void resetFixedFont() { m_fixedFont = false; }
    void setFixedFontValue(bool value) { m_fixedFont = value; }
    bool fixedFont() {return m_fixedFont; }

    const GridPaddings & gridPaddings() {return m_gridPaddings; }
    void setGridPaddings(const GridPaddings &paddings) { m_gridPaddings = paddings; }

    bool operator= (const GlyphProfile &profile)
    {
        return m_key == profile.m_key;
    }

    const GlyphProfile & operator== (const GlyphProfile &profile)
    {
        m_key = profile.m_key;
        m_font = profile.m_font;
        m_fontPath = profile.m_fontPath;
        m_glyphSize = profile.m_glyphSize;
        m_fixedFont = profile.m_fixedFont;

        return *this;
    }

    bool operator < (const GlyphProfile &profile)
    {
        return (m_key < profile.m_key);   
    }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphProfile &profile);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphProfile &profile);
#endif


private:
    ProfileKey m_key;
    QFont m_font;
    QString m_fontPath;
    GridPaddings m_gridPaddings;
    int m_glyphSize;
    bool m_fixedFont;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphProfile &profile)
{
    out << profile.m_key
        << profile.m_glyphSize
        << profile.m_fixedFont
        << profile.m_font
        << profile.m_fontPath
        << profile.m_gridPaddings;

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphProfile &profile)
{
    in >> profile.m_key
       >> profile.m_glyphSize
       >> profile.m_fixedFont
       >> profile.m_font
       >> profile.m_fontPath
       >> profile.m_gridPaddings;
    return in;
}
#endif


#endif // GLYPHPROFILE_H
