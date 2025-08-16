#include "dbmanager.h"
#include "appcontext.h"

DbManager::DbManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_dbDir(appContext->appUserDir())
    , m_dbFile(appContext->appUserDir() + "/" + "glyph.sqlite")
{
    initDb();    
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

void DbManager::enableWalMode()
{
    if (m_db.driverName() == "QSQLITE" && m_db.isOpen()) {
        QSqlQuery query(m_db);
        if (!query.exec("PRAGMA journal_mode=WAL")) {
            qWarning() << "Failed to set WAL mode:" << query.lastError();
        }
        if (!query.exec("PRAGMA synchronous=NORMAL")) {
            qWarning() << "Failed to set synchronous mode:" << query.lastError();
        }
    }
}

bool DbManager::initDb(const QString &dbFile)
{
    if (QSqlDatabase::connectionNames().contains("main"))
        return true;

    m_db = QSqlDatabase::addDatabase("QSQLITE", "main");
    m_db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=30000");
    m_db.setDatabaseName(dbFile == QString() ? m_dbFile : dbFile);

    if (!m_db.open())
    {
        qWarning() << "Can't open database" << m_dbFile;
        return false;
    }

    qDebug() << __FILE__ << __LINE__ << m_dbFile << m_db.isOpen();

    enableWalMode();

    return true;
}
