#ifndef DBCORE_H
#define DBCORE_H

#include <QObject>
#include <QString>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "GlyphCore_global.h"

class AppData;
class AppContext;

class GLYPHCORE_EXPORT DbCore : public QObject
{
    Q_OBJECT
public:
    DbCore(AppContext *appContext);
    ~DbCore();

    void setAppData(AppData *appData);

    bool createDatabase(const QString &dbPathName);

private:
    void initValues();

    QString m_dbPath;

    QSqlDatabase m_db;

    AppData *m_appData;
    AppContext *m_appContext;
};

#endif // DBCORE_H
