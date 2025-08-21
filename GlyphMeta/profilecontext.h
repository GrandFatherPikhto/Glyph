#ifndef GLYPHPROFILE_H_
#define GLYPHPROFILE_H_

#include <QString>
#include <QFont>
#include <QSharedPointer>
#include <QVariant>

class GlyphContext;

class ProfileContext {

public:
    explicit ProfileContext(const QString &name = QString(), int width = -1, int height = -1, int glyphSize=-1, int fontSize = -1, int id = -1, int fontId = -1)
        : m_id(id)
        , m_fontId(fontId)
        , m_name(name)
        , m_gridWidth(width)
        , m_gridHeight(height)
        , m_glyphSize(glyphSize)
        , m_fontSize(fontSize)
        , m_gridLeft(0)
        , m_gridTop(0)
        , m_gridRight(0)
        , m_gridBottom(0)
    {
    }

    ProfileContext(const ProfileContext &profile)
        : m_id(profile.m_id)
        , m_fontId(profile.m_fontId)
        , m_name(profile.m_name)
        , m_glyphSize(profile.m_glyphSize)
        , m_fontSize(profile.m_fontSize)
        , m_gridWidth(profile.m_gridWidth)
        , m_gridHeight(profile.m_gridHeight)
        , m_gridLeft(profile.m_gridLeft)
        , m_gridTop(profile.m_gridTop)
        , m_gridRight(profile.m_gridRight)
        , m_gridBottom(profile.m_gridBottom)
    {}

    ProfileContext(const ProfileContext *profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_name(profile->m_name)
        , m_gridWidth(profile->m_gridWidth)
        , m_gridHeight(profile->m_gridHeight)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
        , m_gridLeft(profile->m_gridLeft)
        , m_gridTop(profile->m_gridTop)
        , m_gridRight(profile->m_gridRight)
        , m_gridBottom(profile->m_gridBottom)
    {}

    ProfileContext(QSharedPointer<ProfileContext> profile)
        : m_id(profile->m_id)
        , m_fontId(profile->m_fontId)
        , m_name(profile->m_name)
        , m_glyphSize(profile->m_glyphSize)
        , m_fontSize(profile->m_fontSize)
        , m_gridWidth(profile->m_gridWidth)
        , m_gridHeight(profile->m_gridHeight)
        , m_gridLeft(profile->m_gridLeft)
        , m_gridTop(profile->m_gridTop)
        , m_gridRight(profile->m_gridRight)
        , m_gridBottom(profile->m_gridBottom)
    {}

    ~ProfileContext() {}

    bool isValid() const
    {
        return (
            m_gridWidth > 0
            && m_gridHeight > 0
            && m_glyphSize > 0
        );
    }

    int id() const { return m_id; }
    const QString & name() const { return m_name; }

    int fontId () const { return m_fontId; }

    int glyphSize() const { return m_glyphSize; }
    int fontSize() const { return m_fontSize; }

    int gridWidth() const {return m_gridWidth; }
    int gridHeight() const {return m_gridHeight; }
    int gridLeft() const { return m_gridLeft; }
    int gridTop() const { return m_gridTop; }
    int gridRight() const { return m_gridRight; }
    int gridBottom() const { return m_gridBottom; }

    void setId(int value) { m_id = value; }
    void setName(const QString &value) { m_name = value; }
    void setFontId(int value) { m_fontId = value; }
    void setGlyphSize (int value) { m_glyphSize = value; }
    void setFontSize (int value) { m_fontSize = value; }
    void setGridWidth(int value) { m_gridWidth = value; }
    void setGridHeight(int value) { m_gridHeight = value; }
    void setGridLeft(int value) { m_gridLeft = value; }
    void setGridTop(int value) { m_gridTop = value; }
    void setGridRight(int value) { m_gridRight = value; }
    void setGridBottom(int value) { m_gridBottom = value; }

    bool operator== (const ProfileContext &profile) const
    {
        return (
               m_id == profile.m_id
            && m_fontId == profile.m_fontId
            && m_gridWidth == profile.m_gridWidth
            && m_gridHeight == profile.m_gridHeight
            && m_name == profile.m_name
            && m_glyphSize == profile.m_glyphSize
            && m_fontSize == profile.m_fontSize
            && m_gridLeft == profile.m_gridLeft
            && m_gridTop == profile.m_gridTop
            && m_gridRight == profile.m_gridRight
            && m_gridBottom == profile.m_gridBottom
        );
    }

    bool operator!= (const ProfileContext &profile) const
    {
        return (
               m_id != profile.m_id
            || m_name != profile.m_name
            || m_fontId != profile.m_fontId
            || m_gridWidth != profile.m_gridWidth
            || m_gridHeight != profile.m_gridHeight
            || m_glyphSize != profile.m_glyphSize
            || m_fontSize != profile.m_fontSize
            || m_gridLeft != profile.m_gridLeft
            || m_gridTop != profile.m_gridTop
            || m_gridRight != profile.m_gridRight
            || m_gridBottom != profile.m_gridBottom
        );
    }

    const ProfileContext & operator= (const ProfileContext &profile)
    {
        if (this != &profile)
        {
            m_id = profile.m_id;
            m_name = profile.m_name;
            m_fontId = profile.m_fontId;
            m_gridWidth = profile.m_gridWidth;
            m_gridHeight = profile.m_gridHeight;
            m_glyphSize = profile.m_glyphSize;
            m_fontSize = profile.m_fontSize;
            m_gridLeft = profile.m_gridLeft;
            m_gridTop = profile.m_gridTop;
            m_gridRight = profile.m_gridRight;
            m_gridBottom = profile.m_gridBottom;
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
            map["font_id"] = m_fontId;
            map["glyph_size"] = m_glyphSize;
            map["font_size"] = m_fontSize;
            map["grid_width"] = m_gridWidth;
            map["grid_height"] = m_gridHeight;
            map["grid_left"] = m_gridLeft;
            map["grid_top"] = m_gridTop;
            map["grid_right"] = m_gridRight;
            map["grid_bottom"] = m_gridBottom;

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
    int m_gridWidth;
    int m_gridHeight;
    int m_glyphSize;
    int m_fontSize;
    int m_gridLeft;
    int m_gridTop;
    int m_gridRight;
    int m_gridBottom;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const ProfileContext &profile)
{
    out << profile.m_id
        << profile.m_name
        << profile.m_fontId
        << profile.m_gridWidth
        << profile.m_gridHeight
        << profile.m_glyphSize
        << profile.m_fontSize
        << profile.m_gridLeft
        << profile.m_gridTop
        << profile.m_gridRight
        << profile.m_gridBottom;

    return out;
}

inline QDataStream & operator >> (QDataStream &in, ProfileContext &profile)
{
    in  >> profile.m_id
        >> profile.m_name
        >> profile.m_fontId
        >> profile.m_gridWidth
        >> profile.m_gridHeight
        >> profile.m_glyphSize
        >> profile.m_fontSize
        >> profile.m_gridLeft
        >> profile.m_gridTop
        >> profile.m_gridRight
        >> profile.m_gridBottom;

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
        << ", Width: "
        << profile.gridWidth()
        << ", Height: "
        << profile.gridHeight()
        << ", Font ID: "
        << profile.fontId()
        << ", Glyph Size: "
        << profile.glyphSize()
        << ", Font Size: "
        << profile.fontSize()
        << ", Valid: "
        << profile.isValid ()
        << ", Padding: ("
        << profile.gridLeft()
        << ", "
        << profile.gridTop()
        << ", "
        << profile.gridRight()
        << ", "
        << profile.gridBottom()
        << "))";

    return debug;
}
#endif

Q_DECLARE_METATYPE(ProfileContext)

#endif // GLYPHPROFILE_H_
