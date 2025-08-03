#ifndef PROFILEKEY_H
#define PROFILEKEY_H

#include <QString>
#include <QDebug>
#include <QDataStream>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT ProfileKey
{
public:
    ProfileKey(const QString &name, int bitmapDimension) : m_name(name), m_bitmapDimension(m_bitmapDimension) {};

    int bitmapDimension() const { return m_bitmapDimension; }
    void setBitmapDimension(int value) { m_bitmapDimension = value; }

    const QString & name() const { return m_name; }
    void setName(const QString &value) { m_name = value; }

    bool operator== (const ProfileKey &key)
    {
        return (m_name == key.m_name)
            && (m_bitmapDimension = key.m_bitmapDimension);
    }

    bool operator< (const ProfileKey &key)
    {
        if (m_bitmapDimension != key.m_bitmapDimension)
            return m_bitmapDimension < key.m_bitmapDimension;
        return m_name < key.m_name;
    }

    const ProfileKey & operator= (const ProfileKey &key)
    {
        m_bitmapDimension = key.m_bitmapDimension;
        m_name = key.m_name;

        return *this;
    }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const ProfileKey &key);
    friend inline QDataStream & operator>>(QDataStream &in, ProfileKey &key);
#endif

private:
    QString m_name;
    int m_bitmapDimension;    
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const ProfileKey &key)
{
    out << key.m_name
        << key.m_bitmapDimension;

    return out;
}

inline QDataStream & operator>>(QDataStream &in, ProfileKey &key)
{
    in >> key.m_name
       >> key.m_bitmapDimension;
       
    return in;
}
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const ProfileKey &key)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() 
        << "ProfileKey("
        << key.name()
        << ", "
        << key.bitmapDimension()
        << ")";

    return debug;
}
#endif

Q_DECLARE_METATYPE(ProfileKey)

#endif // PROFILEKEY_H
