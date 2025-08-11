#ifndef CHARMAPKEY_H_
#define CHARMAPKEY_H_

#include <QString>

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT CharmapKey {
public:
    CharmapKey(const QString &nameValue = "", const QString &opValue = "=") : name(nameValue), op(opValue) {

    }

    CharmapKey(const CharmapKey &key)
        : name(key.name)
        , op(key.op)
        {}

    inline QString bindName(int counter = -1, const QString& prefix = "") const
    {
        QString bindName = QString(":%1").arg(name).replace(".", "_");
        
        if (!prefix.isEmpty())
            bindName.prepend(prefix + "_");
        
        if (counter >= 0)
            bindName += QString("_%1").arg(counter);
        
        return bindName;
    }

    inline const CharmapKey & operator= (const CharmapKey& key)
    {
        name = key.name;
        op = key.op;

        return *this;
    }

    inline bool operator== (const CharmapKey &key) const
    {
        return (name == key.name) 
            && (op == key.op);
    }

    inline bool operator< (const CharmapKey &key) const
    {
        if (name != key.name)
            return name < key.name;
        return op < key.op;
    }

    QString name;
    QString op;

private:

};

#endif // CHARMAPKEY_H_
