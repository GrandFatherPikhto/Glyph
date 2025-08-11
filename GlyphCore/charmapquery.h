#ifndef WHERECLAUSE_H_
#define WHERECLAUSE_H_

#include <QObject>
#include <QPointer>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QItemSelectionModel>

#include "GlyphCore_global.h"
#include "charmapkey.h"

class AppContext;

class GLYPHCORE_EXPORT CharmapQuery : public QObject
{
    Q_OBJECT

public:
    explicit CharmapQuery(AppContext *appContext);
    ~CharmapQuery();

    void initDb ();

    // Добавляет условие из QListView (для IN-фильтров)
    bool setFieldCondition(const QString &sqlFieldName, const QVector<int> &values, const QString &op = "=");
    void setTextFilter(const QString &chars);
    void setMSBFilter(int value);
    
    QString updateQuery();
    bool prepareQuery();
    bool execQuery(QSqlQuery &query);

    QStringList find(const CharmapKey &key);
    bool remove(const CharmapKey &key);

    bool isEmpty() const {
        return m_items.isEmpty();
    }

private:
    QMap<CharmapKey, QStringList> m_items;
    QMap<QString, QVariant> m_bindValues;

    QString m_sortCondition;

    QString m_startQuery;
    QString m_sqlBegin;
    QString m_tableName;

    QString m_charsFilter;

    int m_msb;

    AppContext *m_appContext;
};

#endif // WHERECLAUSE_H
