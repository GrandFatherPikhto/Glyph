#include "dbcore.h"
#include "appcontext.h"
#include "appdata.h"

#include <QSqlDatabase>
#include <QStandardPaths>
#include <QDir>

DbCore::DbCore(AppContext *appContext)
    : QObject{appContext}
    , m_dbPath(QString())
    , m_appContext(appContext)
{
    Q_ASSERT(m_appContext->appData() != nullptr);
    m_appData = m_appContext->appData();
}

DbCore::~DbCore()
{
    m_db.close();
}

bool DbCore::createDatabase(const QString &dbPathName)
{
    if (m_db.open())
    {
        m_db.close();
    }

    // Создание файла БД в этом каталоге
    m_dbPath = dbPathName + "/database.sqlite";
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        qCritical() << "Не удалось открыть базу данных:" << m_db.lastError().text();
        return false;
    }

    // Создаем таблицу
    QSqlQuery query;
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS unicode_data ("
        "unicode INTEGER NOT NULL, "       // uint32
        "category INTEGER NOT NULL, "      // uint8
        "script INTEGER NOT NULL, "        // uint16
        "decomposition INTEGER NOT NULL"   // uint8
        ");";

    if (!query.exec(createTableQuery)) {
        qCritical() << "Can't create table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database and tables created at: " << m_dbPath;

    return true;
}

void DbCore::initValues()
{

}

void DbCore::setAppData(AppData *appData)
{
    m_appData = appData;
}

