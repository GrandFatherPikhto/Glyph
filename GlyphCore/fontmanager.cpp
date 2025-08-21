#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

#include "fontmanager.h"
#include "fontloader.h"
#include <windows.h>
#include <shlobj.h>

#include "appcontext.h"
#include "fontutils.h"
#include "dbmanager.h"

FontManager::FontManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_dbManager(appContext->dbManager())
    , m_fontLoader(nullptr)
    , m_tableName(QString("fonts"))
{
    setupValues ();
    setDefaultFontPath();
    restoreFontManagerSettings ();
    createTable();
    setupSignals ();
}

FontManager::~FontManager()
{
    saveFontManagerSettings ();
    if (m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();
    }
}

void FontManager::setupValues()
{

}

void FontManager::setupSignals()
{
    connect(this, &FontManager::updateFontDatabase, this, [=]{
        startAsyncFontLoading();
    });

    connect(this, &FontManager::loadingProgress, this, [=](int progress, int total){
        qDebug() << "Loading" << progress << total;
    });

    connect(this, &FontManager::loadingFinished, this, [=](int total){
        qDebug() << "Finished:" << total;
    });

    connect(this, &FontManager::addedFontContext, this, [=](const FontContext &context){
        qDebug() << "Added Context" << context;
    });

    connect(this, &FontManager::changeFontContext, this, &FontManager::setFontContext);
}

void FontManager::startAsyncFontLoading()
{
    // Останавливаем предыдущую операцию
    if (m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();

        // Удаляем старый loader (если был)
        if (m_fontLoader) {
            m_fontLoader->deleteLater();
            m_fontLoader = nullptr;
        }
    }

    // Создаем новый loader без родителя
    m_fontLoader = new FontLoader(m_tableName, m_dbManager->dbFile());

    // Настраиваем соединения перед перемещением в поток
    connect(m_fontLoader, &FontLoader::progress, this, &FontManager::loadingProgress);
    connect(m_fontLoader, &FontLoader::finished, this, &FontManager::loadingFinished);
    connect(m_fontLoader, &FontLoader::addedFontContext, this, &FontManager::addedFontContext);

    // Перемещаем в поток
    m_fontLoader->moveToThread(&m_workerThread);

    // Соединение для запуска задачи (после moveToThread!)
    connect(this, &FontManager::startLoading, m_fontLoader, &FontLoader::loadFonts, Qt::QueuedConnection);

    // Удаление при завершении потока
    // connect(&m_workerThread, &QThread::finished, m_fontLoader, &QObject::deleteLater);
    connect(&m_workerThread, &QThread::finished, this, [this]() {
        if (m_fontLoader) {
            m_fontLoader->deleteLater();
            m_fontLoader = nullptr;
        }
        // emit loadingFinished();
    });

    // Запускаем поток
    m_workerThread.start();

    qDebug() << __FILE__ << __LINE__ << m_fontDirs;
    emit startLoading(m_fontDirs);
}


bool FontManager::setDefaultFontPath()
{
    QString defaultFontPath = ::winDefaultFontPath();
    qDebug() << defaultFontPath << m_fontDirs.contains(defaultFontPath);
    if (!m_fontDirs.contains(defaultFontPath))
    {
        m_fontDirs << defaultFontPath;
        qDebug() << m_fontDirs;
        return true;
    }

    return false;
}

bool FontManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen())
    {
        qWarning()  << __FILE__ << __LINE__ << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    QString createTableQuery =
        QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "path TEXT NOT NULL, "
            "family TEXT NOT NULL, "
            "style TEXT, "
            "system TEXT, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
            "UNIQUE(name, path)"
            ");"
            ).arg(m_tableName);

    if (!query.exec(createTableQuery)) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }

    return true;
}

bool FontManager::fontContextById(int id, FontContext &context)
{
    if (id < 0)
        return false;
    
    QSqlDatabase db = QSqlDatabase::database("main");
    // QSqlDatabase db = m_appContext->dbManager()->db();

    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    
    QString sql = QString("SELECT id, name, path, family, style, system, created_at FROM %1 WHERE id = :id").arg(m_tableName);

    if(!query.prepare(sql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare" << query.lastQuery() << ", With Id: " << id << ", Error:" << query.lastError();
        return false;
    }

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Error exec query" << query.lastQuery() << ", With Id: " << id << ", Error:" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        qWarning() << __FILE__ << __LINE__ << "Not find records" << query.lastQuery() << ", With Id:" << id << ", Error: " << query.lastError();
        return false;
    }

    try {
        context.setId(query.value("id").toInt());
        context.setPath(query.value("path").toString());
        context.setName(query.value("name").toString());
        context.setFamily(query.value("family").toString());
        context.setStyle(query.value("style").toString());
        context.setSystem(query.value("system").toString());
    } catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}

void FontManager::saveFontManagerSettings()
{
    QSettings settings(this);
    settings.beginGroup("FontManager");
    settings.setValue("fontContext", m_fontContext);
    settings.endGroup();
}

void FontManager::restoreFontManagerSettings()
{
    QSettings settings(this);
    settings.beginGroup("FontManager");
    m_fontContext = settings.value("fontContext", FontContext()).value<FontContext>();
    settings.endGroup();
}
