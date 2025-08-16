#ifndef FONTCONTEXT_H
#define FONTCONTEXT_H

#include <QFont>
#include <QString>
#include <QVariant>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT FontContext {
public:
    explicit FontContext(const QString & path = QString(), const QString &name = QString(), const QString &family = QString(), const QString &style = QString(), const QString &system = QString(), int id = -1)
        : m_path(path)
        , m_name(name)
        , m_family(family)
        , m_style(style)
        , m_system(system)
        , m_id(id)
    {}

    FontContext(const FontContext &context)
        : m_path(context.m_path)
        , m_name(context.m_name)
        , m_family(context.m_family)
        , m_style(context.m_style)
        , m_system(context.m_system)
        , m_id(context.m_id)
    {}

    ~FontContext() {}

    inline const QString & path() const { return m_path; }
    inline const QString & name() const { return m_name; }
    inline const QString & family() const { return m_family; }
    inline const QString & style() const { return m_style; }
    inline const QString & system() const { return m_system; }
    inline const int id() const { return m_id; }

    bool isValid() const { return m_path != QString() && m_family != QString(); }

    inline void setName(const QString &value) { m_name = value; }
    inline void setFamily(const QString &value) { m_family = value; }
    inline void setStyle(const QString &value) { m_style = value; }
    inline void setPath(const QString &value) { m_path = value; }
    inline void setSystem(const QString &value) { m_system = value; }
    inline void setId(int value) { m_id = value; }

    inline bool operator== (const FontContext &context) const
    {
        return (
               m_id == context.m_id
            && m_name == context.m_name
            && m_family == context.m_family
            && m_style == context.m_style
            && m_system == context.m_system
            && m_path == context.m_path
            );
    }

    inline bool operator!= (const FontContext &context) const
    {
        return (
               m_id != context.m_id
            || m_name != context.m_name
            || m_family != context.m_family
            || m_style != context.m_style
            || m_system != context.m_system
            || m_path != context.m_path
            );
    }

    inline const FontContext & operator= (const FontContext &context)
    {
        m_id = context.m_id;
        m_name = context.m_name;
        m_family = context.m_family;
        m_style = context.m_style;
        m_system = context.m_system;
        m_path = context.m_path;

        return *this;
    }

    operator QVariant() const {
        if(QMetaType::fromName("GlyphContext").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridOffsets не зарегистрирован
            QVariantMap map;

            map["id"] = m_id;
            map["name"] = m_name;
            map["family"] = m_family;
            map["system"] = m_system;
            map["style"] = m_style;
            map["path"] = m_path;

            return map;
        }
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const FontContext &context);
    friend QDataStream & operator >> (QDataStream &in, FontContext &context);
#endif

private:
    int m_id;
    QString m_path;
    QString m_name;
    QString m_system;
    QString m_family;
    QString m_style;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const FontContext &context)
{
    out << context.m_id
        << context.m_name
        << context.m_system
        << context.m_path
        << context.m_family
        << context.m_style;

    return out;
}

inline QDataStream & operator >> (QDataStream &in, FontContext &context)
{
    in  >> context.m_id
        >> context.m_name
        >> context.m_system
        >> context.m_path
        >> context.m_family
        >> context.m_style;

    return in;
}

#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const FontContext &context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "FontContext( Id: "
                    << context.id()
                    << ", Name: "
                    << context.name()
                    << ", System: "
                    << context.system()
                    << ", Family: "
                    << context.family()
                    << ", Style: "
                    << context.style()
                    << ", Path: "
                    << context.path()
                    << ")";

    return debug;
}
#endif // QT_NO_DEBUG_OUTPUT

Q_DECLARE_METATYPE(FontContext)

#endif // FONTCONTEXT_H
