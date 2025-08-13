#ifndef GLYPHPROFILE_H_
#define GLYPHPROFILE_H_

#include <QString>
#include <QFont>
#include <QSharedPointer>
#include <QVariant>

class GlyphContext;

class ProfileContext {

public:
    explicit ProfileContext(const QString &name = QString(), int bitmapDimension = -1, int glyphSize=-1, const QFont &font = QFont(), const QString &fontPath = QString(), int id = -1, bool temporary = true)
        : m_id(id)
        , m_name(name)
        , m_font(font)
        , m_fontPath(fontPath)
        , m_temporary(temporary)
        , m_bitmapDimension(bitmapDimension)
        , m_glyphSize(glyphSize)
        , m_paddingLeft(0)
        , m_paddingTop(0)
        , m_paddingRight(0)
        , m_paddingBottom(0)
    {
    }

    ProfileContext(const ProfileContext &profile)
        : m_id(profile.m_id)
        , m_name(profile.m_name)
        , m_bitmapDimension(profile.m_bitmapDimension)
        , m_glyphSize(profile.m_glyphSize)
        , m_font(profile.m_font)
        , m_fontPath(profile.m_fontPath)
        , m_temporary(profile.m_temporary)
        , m_paddingLeft(profile.m_paddingLeft)
        , m_paddingTop(profile.m_paddingTop)
        , m_paddingRight(profile.m_paddingRight)
        , m_paddingBottom(profile.m_paddingBottom)
    {}

    ProfileContext(const ProfileContext *profile)
        : m_id(profile->m_id)
        , m_name(profile->m_name)
        , m_bitmapDimension(profile->m_bitmapDimension)
        , m_glyphSize(profile->m_glyphSize)
        , m_font(profile->m_font)
        , m_fontPath(profile->m_fontPath)
        , m_temporary(profile->m_temporary)
        , m_paddingLeft(profile->m_paddingLeft)
        , m_paddingTop(profile->m_paddingTop)
        , m_paddingRight(profile->m_paddingRight)
        , m_paddingBottom(profile->m_paddingBottom)
    {}

    ProfileContext(QSharedPointer<ProfileContext> profile)
        : m_id(profile->m_id)
        , m_name(profile->m_name)
        , m_glyphSize(profile->m_glyphSize)
        , m_bitmapDimension(profile->m_bitmapDimension)
        , m_font(profile->m_font)
        , m_fontPath(profile->m_fontPath)
        , m_temporary(profile->m_temporary)
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
        );
    }

    int id() const { return m_id; }
    int bitmapDimension() const {return m_bitmapDimension; }
    int glyphSize() const { return m_glyphSize; }
    const QFont & font() const { return m_font; }
    const QString & fontPath() const { return m_fontPath; }
    const QString & name() const { return m_name; }
    bool temporary() const { return m_temporary; }

    int paddingLeft() const { return m_paddingLeft; }
    int paddingTop() const { return m_paddingTop; }
    int paddingRight() const { return m_paddingRight; }
    int paddingBottom() const { return m_paddingBottom; }

    void setId(int value) { m_id = value; }
    void setName(const QString &value) { m_name = value; }
    void setBitmapDimension(int value) { m_bitmapDimension = value; }
    void setGlyphSize(int value) { m_glyphSize = value; }
    void setFont(const QFont &value) { m_font = value; }
    void setFontPath(const QString &value) { m_fontPath = value; }
    void setTemporary (bool value = true) { m_temporary = value; }
    void setPaddingLeft(int value) { m_paddingLeft = value; }
    void setPaddingTop(int value) { m_paddingTop = value; }
    void setPaddingRight(int value) { m_paddingRight = value; }
    void setPaddingBottom(int value) { m_paddingBottom = value; }

    bool operator== (const ProfileContext &profile) const
    {
        return (
               m_id == profile.m_id
            && m_bitmapDimension == profile.m_bitmapDimension
            && m_glyphSize == profile.m_glyphSize
            && m_name == profile.m_name
            && m_font == profile.m_font
            && m_temporary == profile.m_temporary
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
            || m_bitmapDimension != profile.m_bitmapDimension
            || m_glyphSize != profile.m_glyphSize
            || m_name != profile.m_name
            || m_font != profile.m_font
            || m_temporary != profile.m_temporary
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
            m_font = profile.m_font;
            m_fontPath = profile.m_fontPath;
            m_bitmapDimension = profile.m_bitmapDimension;
            m_glyphSize = profile.m_glyphSize;
            m_temporary = profile.m_temporary;
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
            map["bitmapDimension"] = m_bitmapDimension;
            map["glyphSize"] = m_glyphSize;
            map["font"] = m_font;
            map["name"] = m_name;
            map["temporary"] = m_temporary;
            map["paddingLeft"] = m_paddingLeft;
            map["paddingTop"] = m_paddingTop;
            map["paddingRight"] = m_paddingRight;
            map["paddingBottom"] = m_paddingBottom;

            return map;
        }
    }


#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const ProfileContext &profile);
    friend QDataStream & operator >> (QDataStream &in, ProfileContext &profile);
#endif

private:
    int m_bitmapDimension;
    int m_glyphSize;

    QFont m_font;
    QString m_fontPath;
    QString m_name;

    bool m_temporary;

    int m_paddingLeft;
    int m_paddingTop;
    int m_paddingRight;
    int m_paddingBottom;
    int m_id;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const ProfileContext &profile)
{
    out << profile.m_id
        << profile.m_name
        << profile.m_bitmapDimension
        << profile.m_glyphSize
        << profile.m_font
        << profile.m_fontPath
        << profile.m_temporary
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
        >> profile.m_bitmapDimension
        >> profile.m_glyphSize
        >> profile.m_font
        >> profile.m_fontPath
        >> profile.m_temporary
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
        << ", temporary: "
        << profile.temporary ()
        << ", Dimension: "
        << profile.bitmapDimension()
        << ", Size:"
        << profile.glyphSize()
        << ", Font: "
        << profile.font()
        << ", Font path: "
        << profile.fontPath ()
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
