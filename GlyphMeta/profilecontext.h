#ifndef GLYPHPROFILE_H_
#define GLYPHPROFILE_H_

#include <QString>
#include <QFont>
#include <QSharedPointer>
#include <QVariant>

class GlyphContext;

class ProfileContext {

public:
    explicit ProfileContext(int id = -1, int fontId = -1, int gridId = -1, const QString &name = QString(), int glyphSize=-1, int fontSize = -1)
        : m_id(id)
        , m_fontId(fontId)
        , m_gridId(gridId)
        , m_name(name)
        , m_glyphSize(glyphSize)
        , m_fontSize(fontSize)
    {
    }

    ProfileContext(const ProfileContext &profile)
        : m_id(profile.m_id)
        , m_fontId(profile.m_fontId)
        , m_gridId(profile.m_gridId)
        , m_name(profile.m_name)
        , m_glyphSize(profile.m_glyphSize)
        , m_fontSize(profile.m_fontSize)
    {}

    ProfileContext(const ProfileContext *profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_gridId(profile->m_gridId)
        , m_name(profile->m_name)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
    {}

    ProfileContext(QSharedPointer<ProfileContext> profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_gridId(profile->m_gridId)
        , m_name(profile->m_name)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
    {}

    ~ProfileContext() {}

    bool isValid() const
    {
        return (
            m_glyphSize > 0
        );
    }

    int id() const { return m_id; }
    int fontId () const { return m_fontId; }
    int gridId() const { return m_gridId; }

    const QString & name() const { return m_name; }

    int glyphSize() const { return m_glyphSize; }
    int fontSize() const { return m_fontSize; }

    void setId(int value) { m_id = value; }
    void setGridId(int value) { m_gridId = value; }
    void setFontId(int value) { m_fontId = value; }
    void setName(const QString &value) { m_name = value; }
    void setGlyphSize (int value) { m_glyphSize = value; }
    void setFontSize (int value) { m_fontSize = value; }

    bool operator== (const ProfileContext &profile) const
    {
        return (
               m_id == profile.m_id
            && m_fontId == profile.m_fontId
            && m_gridId == profile.m_gridId
            && m_name == profile.m_name
            && m_glyphSize == profile.m_glyphSize
            && m_fontSize == profile.m_fontSize
        );
    }

    bool operator!= (const ProfileContext &profile) const
    {
        return (
               m_id != profile.m_id
            || m_fontId != profile.m_fontId
            || m_gridId != profile.m_gridId
            || m_name != profile.m_name
            || m_glyphSize != profile.m_glyphSize
            || m_fontSize != profile.m_fontSize
        );
    }

    const ProfileContext & operator= (const ProfileContext &profile)
    {
        if (this != &profile)
        {
            m_id = profile.m_id;
            m_gridId = profile.m_gridId;
            m_fontId = profile.m_fontId;
            m_name = profile.m_name;
            m_glyphSize = profile.m_glyphSize;
            m_fontSize = profile.m_fontSize;
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
            map["grid_id"] = m_gridId;
            map["font_id"] = m_fontId;
            map["name"] = m_name;
            map["glyph_size"] = m_glyphSize;
            map["font_size"] = m_fontSize;

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
    int m_glyphSize;
    int m_fontSize;
    int m_gridId;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const ProfileContext &profile)
{
    out << profile.m_id
        << profile.m_gridId
        << profile.m_name
        << profile.m_fontId
        << profile.m_glyphSize
        << profile.m_fontSize;

    return out;
}

inline QDataStream & operator >> (QDataStream &in, ProfileContext &profile)
{
    in  >> profile.m_id
        >> profile.m_gridId
        >> profile.m_name
        >> profile.m_fontId
        >> profile.m_glyphSize
        >> profile.m_fontSize;

    return in;
}

#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const ProfileContext &profile)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "ProfileContext(Id: "
        << profile.id()
        << ", Grid Id: "
        << profile.gridId()
        << ", Name: "
        << profile.name()
        << ", Font ID: "
        << profile.fontId()
        << ", Glyph Size: "
        << profile.glyphSize()
        << ", Font Size: "
        << profile.fontSize()
        << ", Valid: "
        << profile.isValid ()
        << ")";

    return debug;
}
#endif

Q_DECLARE_METATYPE(ProfileContext)

#endif // GLYPHPROFILE_H_
