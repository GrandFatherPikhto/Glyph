#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QThread>
#include <QDir>

#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT DbManager : public QObject {
    Q_OBJECT
public:
    explicit DbManager(AppContext *appContext);
    ~DbManager();
    
    bool initDb(const QString &dbFile = QString());
    void enableWalMode();
    
    const QString dbFile() const { return m_dbFile; }

    QSqlDatabase db() { return m_db; }

private:

    AppContext *m_appContext;

    QString m_dbDir;
    QString m_dbFile;
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
