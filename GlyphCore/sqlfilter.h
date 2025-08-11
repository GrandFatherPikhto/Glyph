#ifndef SQLFILTER_H
#define SQLFILTER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QMap>
#include <QPair>
#include <QMutex>

#include "GlyphCore_global.h"

#include "condition.h"

class GLYPHCORE_EXPORT SqlFilter : public QObject
{
    Q_OBJECT
public:
    explicit SqlFilter(QObject *parent = nullptr);
    ~SqlFilter();

    bool addCondition(const QString &name, QSharedPointer<Condition> condition);
    bool removeCondition(QSharedPointer<Condition> condition);
    bool removeCondition(const QString &name);

    bool isFilter ();

    void reset();

    QString prepare();

    const QList<QPair<QString, QVariant>> binds() const { return m_binds; }

    QSharedPointer<Condition> getCondition(const QString &name);

signals:

private:
    QMap<QString, QSharedPointer<Condition>> m_conditions;
    QList<QPair<QString, QVariant>> m_binds;
    QString m_sql;
};

#endif // SQLFILTER_H
