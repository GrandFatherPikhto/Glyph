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

class AppSettings;
class AppContext;

class GLYPHCORE_EXPORT DbCore : public QObject
{
    Q_OBJECT
public:
    DbCore(AppContext *appContext);
    ~DbCore();

    bool createDatabase(const QString &dbPath);
    bool resetTable(const QString &tableName);

signals:
    

private:
    QString getAppDataDbPath();

    void setupValues();
    void setupSignals ();
    
    void releaseDb ();
    bool createTables();

    QString m_dbPath;
    QSqlDatabase m_db;

    AppSettings *m_appSettings;
    AppContext *m_appContext;
};

#endif // DBCORE_H
