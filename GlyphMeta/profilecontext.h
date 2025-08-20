#ifndef GLYPHPROFILE_H_
#define GLYPHPROFILE_H_

#include <QString>
#include <QFont>
#include <QSharedPointer>
#include <QVariant>

#include "fontcontext.h"

class GlyphContext;

class ProfileContext {

public:
    explicit ProfileContext(const QString &name = QString(), int bitmapDimension = -1, int glyphSize=-1, int fontSize = -1, int id = -1, int fontId = -1)
        : m_id(id)
        , m_fontId(fontId)
        , m_name(name)
        , m_bitmapDimension(bitmapDimension)
        , m_glyphSize(glyphSize)
        , m_fontSize(fontSize)
        , m_paddingLeft(0)
        , m_paddingTop(0)
        , m_paddingRight(0)
        , m_paddingBottom(0)
    {
    }

    ProfileContext(const ProfileContext &profile)
        : m_id(profile.m_id)
        , m_fontId(profile.m_fontId)
        , m_name(profile.m_name)
        , m_glyphSize(profile.m_glyphSize)
        , m_fontSize(profile.m_fontSize)
        , m_bitmapDimension(profile.m_bitmapDimension)
        , m_paddingLeft(profile.m_paddingLeft)
        , m_paddingTop(profile.m_paddingTop)
        , m_paddingRight(profile.m_paddingRight)
        , m_paddingBottom(profile.m_paddingBottom)
    {}

    ProfileContext(const ProfileContext *profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_name(profile->m_name)
        , m_bitmapDimension(profile->m_bitmapDimension)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
        , m_paddingLeft(profile->m_paddingLeft)
        , m_paddingTop(profile->m_paddingTop)
        , m_paddingRight(profile->m_paddingRight)
        , m_paddingBottom(profile->m_paddingBottom)
    {}

    ProfileContext(QSharedPointer<ProfileContext> profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_name(profile->m_name)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
        , m_bitmapDimension(profile->m_bitmapDimension)
        , m_paddingLeft(profile->m_paddingLeft)
        , m_paddingTop(profile->m_paddingTop)
        , m_paddingRight(profile->m_paddingRight)
        , m_paddingBottom(profile->m_paddingBottom)
    {}

    ~ProfileContext() {}

    bool isValid() const
    {
        return (
            !m_name.isEmpty()
            && m_bitmapDimension > 0
            && m_glyphSize > 0
            && m_fontId >= 0
        );
    }

    int id() const { return m_id; }
    const QString & name() const { return m_name; }

    int fontId () const { return m_fontId; }

    int bitmapDimension() const {return m_bitmapDimension; }
    int glyphSize() const { return m_glyphSize; }
    int fontSize() const { return m_fontSize; }

    int paddingLeft() const { return m_paddingLeft; }
    int paddingTop() const { return m_paddingTop; }
    int paddingRight() const { return m_paddingRight; }
    int paddingBottom() const { return m_paddingBottom; }

    void setId(int value) { m_id = value; }
    void setName(const QString &value) { m_name = value; }
    void setFontId(int value) { m_fontId = value; }
    void setBitmapDimension(int value) { m_bitmapDimension = value; }
    void setGlyphSize (int value) { m_glyphSize = value; }
    void setFontSize (int value) { m_fontSize = value; }
    void setPaddingLeft(int value) { m_paddingLeft = value; }
    void setPaddingTop(int value) { m_paddingTop = value; }
    void setPaddingRight(int value) { m_paddingRight = value; }
    void setPaddingBottom(int value) { m_paddingBottom = value; }

    bool operator== (const ProfileContext &profile) const
    {
        return (
               m_id == profile.m_id
            && m_fontId == profile.m_fontId
            && m_bitmapDimension == profile.m_bitmapDimension
            && m_name == profile.m_name
            && m_glyphSize == profile.m_glyphSize
            && m_fontSize == profile.m_fontSize
            && m_paddingLeft == profile.m_paddingLeft
            && m_paddingTop == profile.m_paddingTop
            && m_paddingRight == profile.m_paddingRight
            && m_paddingBottom == profile.m_paddingBottom
        );
    }

    bool operator!= (const ProfileContext &profile) const
    {
        return (
               m_id != profile.m_id
            || m_name != profile.m_name
            || m_fontId != profile.m_fontId
            || m_bitmapDimension != profile.m_bitmapDimension
            || m_glyphSize != profile.m_glyphSize
            || m_fontSize != profile.m_fontSize
            || m_paddingLeft != profile.m_paddingLeft
            || m_paddingTop != profile.m_paddingTop
            || m_paddingRight != profile.m_paddingRight
            || m_paddingBottom != profile.m_paddingBottom
        );
    }

    const ProfileContext & operator= (const ProfileContext &profile)
    {
        if (this != &profile)
        {
            m_id = profile.m_id;
            m_name = profile.m_name;
            m_fontId = profile.m_fontId;
            m_bitmapDimension = profile.m_bitmapDimension;
            m_glyphSize = profile.m_glyphSize;
            m_fontSize = profile.m_fontSize;
            m_paddingLeft = profile.m_paddingLeft;
            m_paddingTop = profile.m_paddingTop;
            m_paddingRight = profile.m_paddingRight;
            m_paddingBottom = profile.m_paddingBottom;
        }

        return *this;
    }

    // Для QVariant/QSettings
    operator QVariant() const {
        if(QMetaType::fromName("ProfileContext").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridPaddings не зарегистрирован
            QVariantMap map;

            map["id"] = m_id;
            map["name"] = m_name;
            map["bitmap_dimension"] = m_bitmapDimension;
            map["font_id"] = m_fontId;
            map["glyph_size"] = m_glyphSize;
            map["font_size"] = m_fontSize;
            map["padding_left"] = m_paddingLeft;
            map["padding_top"] = m_paddingTop;
            map["padding_right"] = m_paddingRight;
            map["padding_bottom"] = m_paddingBottom;

            return map;
        }
    }


#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const ProfileContext &profile);
    friend QDataStream & operator >> (QDataStream &in, ProfileContext &profile);
#endif

private:
    QString m_name;

    int m_id;
    int m_fontId;
    int m_bitmapDimension;
    int m_glyphSize;
    int m_fontSize;
    int m_paddingLeft;
    int m_paddingTop;
    int m_paddingRight;
    int m_paddingBottom;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const ProfileContext &profile)
{
    out << profile.m_id
        << profile.m_name
        << profile.m_fontId
        << profile.m_bitmapDimension
        << profile.m_glyphSize
        << profile.m_fontSize
        << profile.m_paddingLeft
        << profile.m_paddingTop
        << profile.m_paddingRight
        << profile.m_paddingBottom;

    return out;
}

inline QDataStream & operator >> (QDataStream &in, ProfileContext &profile)
{
    in  >> profile.m_id
        >> profile.m_name
        >> profile.m_fontId
        >> profile.m_bitmapDimension
        >> profile.m_glyphSize
        >> profile.m_fontSize
        >> profile.m_paddingLeft
        >> profile.m_paddingTop
        >> profile.m_paddingRight
        >> profile.m_paddingBottom;

    return in;
}

#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const ProfileContext &profile)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "ProfileContext(Id: "
        << profile.id()
        << ", Name: "
        << profile.name()
        << ", Dimension: "
        << profile.bitmapDimension()
        << ", Font ID: "
        << profile.fontId()
        << ", Glyph Size: "
        << profile.glyphSize()
        << ", Font Size: "
        << profile.fontSize()
        << ", Valid: "
        << profile.isValid ()
        << ", Padding: ("
        << profile.paddingLeft()
        << ", "
        << profile.paddingTop()
        << ", "
        << profile.paddingRight()
        << ", "
        << profile.paddingBottom()
        << "))";

    return debug;
}
#endif

Q_DECLARE_METATYPE(ProfileContext)

#endif // GLYPHPROFILE_H_
