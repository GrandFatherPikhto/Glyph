#include "dbcore.h"
#include "appcontext.h"
#include "appsettings.h"

#include <QSqlDatabase>
#include <QStandardPaths>
#include <QDir>

DbCore::DbCore(AppContext *appContext)
    : QObject{appContext}
    , m_dbPath(QString())
    , m_appContext(appContext)
    , m_appSettings(nullptr)
{
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &DbCore::setupValues);
}

DbCore::~DbCore()
{
    m_db.close();
}

bool DbCore::createDatabase(const QString &dbPath)
{
    releaseDb();

    // Подключаемся к базе данных SQLite (файл будет создан автоматически)
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    m_dbPath = dbPath;

    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }

    return true;
}

void DbCore::setupValues()
{
    Q_ASSERT(m_appContext->appSettings() != nullptr);
    m_appSettings = m_appContext->appSettings();

    QString dbPath = getAppDataDbPath();
    createDatabase(dbPath);
    createTables();
}

void DbCore::setupSignals ()
{

}

bool DbCore::createTables ()
{

    return true;
}

bool DbCore::resetTable(const QString &tableName)
{
    QSqlQuery query(m_db);
    
    // 1. Получаем структуру таблицы (CREATE TABLE ...)
    query.prepare("SELECT sql FROM sqlite_master WHERE type='table' AND name=?");
    query.addBindValue(tableName);
    if (!query.exec() || !query.next()) {
        qWarning() << "Failed to get table schema:" << query.lastError().text();
        return false;
    }
    QString createTableSql = query.value(0).toString();
    
    // 2. Удаляем и пересоздаём таблицу
    m_db.transaction(); // Начинаем транзакцию
    if (!query.exec("DROP TABLE " + tableName)) {
        qWarning() << "Failed to drop table:" << query.lastError().text();
        m_db.rollback();
        return false;
    }

    if (!query.exec(createTableSql)) {
        qWarning() << "Failed to recreate table:" << query.lastError().text();
        m_db.rollback();
        return false;
    }

    m_db.commit(); // Подтверждаем изменения
    qDebug() << "Table" << tableName << "completely reset (DROP + CREATE)";

    return true;
}

void DbCore::releaseDb()
{
    if(m_db.isOpen())
    {
        m_db.close();
    }
}

QString DbCore::getAppDataDbPath()
{
    // Получение пути к AppData/Local/<AppName>
    QString appDataPath = m_appSettings->appDataPath();

    // Пример: создание файла БД в этом каталоге
    QString dbPath = appDataPath + "/database.sqlite";

    return dbPath;
}

