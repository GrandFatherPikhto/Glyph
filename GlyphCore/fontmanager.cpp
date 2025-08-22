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
    ::getWinSystemFonts(m_sysfonts);
    ::getWinSystemFonts(m_sysfonts, HKEY_CURRENT_USER);

    setupValues ();
    setDefaultFontPath();
    restoreFontManagerSettings ();
    createTable();
    setupSignals ();
}

FontManager::~FontManager()
{
    saveFontManagerSettings ();
    resetLoadingThread();
}

void FontManager::setupValues()
{

}

void FontManager::setupSignals()
{
    connect(this, &FontManager::updateFontDatabase, this, [=]() {
        // startAsyncFontLoading();
        loadFonts(m_fontDirs);
    });

    connect(this, &FontManager::loadingStart, this, [=](int total){
        // qDebug() << "Start loading" << total;
    });

    connect(this, &FontManager::loadingProgress, this, [=](int progress, int total){
        // qDebug() << "Loading" << progress << total;
    });

    connect(this, &FontManager::loadingFinished, this, [=](int total){
        // qDebug() << "Finished:" << total;
        resetLoadingThread();
    });

    connect(this, &FontManager::loadFontContext, this, [=](const FontContext &context){
        // qDebug() << "Added Context" << context;
    });

    connect(this, &FontManager::changeFontContext, this, &FontManager::setFontContext);
}

void FontManager::resetLoadingThread()
{
    if (m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();

        // Удаляем старый loader (если был)
        if (m_fontLoader) {
            m_fontLoader->deleteLater();
            m_fontLoader = nullptr;
        }
    }
}

void FontManager::startAsyncFontLoading()
{
    resetLoadingThread();

    // Создаем новый loader без родителя
    m_fontLoader = new FontLoader(m_tableName, m_dbManager->dbFile());

    // Настраиваем соединения перед перемещением в поток
    connect(m_fontLoader, &FontLoader::start, this, &FontManager::loadingStart);
    connect(m_fontLoader, &FontLoader::progress, this, &FontManager::loadingProgress);
    connect(m_fontLoader, &FontLoader::finished, this, &FontManager::loadingFinished);
    connect(m_fontLoader, &FontLoader::append, this, &FontManager::loadFontContext);

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
    });

    // Запускаем поток
    m_workerThread.start();

    emit startLoading(m_fontDirs);
}


bool FontManager::setDefaultFontPath()
{
    QString defaultFontPath = ::winDefaultFontPath();
    if (!m_fontDirs.contains(defaultFontPath))
    {
        m_fontDirs << defaultFontPath;
        // qDebug() << m_fontDirs;
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
            "family TEXT, "
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

int FontManager::totalCounter(const QStringList &list)
{
    int counter = 0;
    for (int i = 0; i < list.size(); i++)
    {
        QString path = list.at(i);
        QDir dir(path);
        if (dir.exists())
        {
            dir.setNameFilters(m_filters);
        }
        QDir fontDir(QDir::toNativeSeparators(path));
        const auto files = dir.entryList(QDir::Files);
        counter += files.size();
    }

    return counter;
}

void FontManager::loadFonts(const QStringList &fontDirs)
{
    m_counter = 0;
    m_total = 0;

    resetLists();

    emit startLoading(fontDirs);

    for (int i = 0; i < fontDirs.size(); ++i) {
        QDir fontsDir(QDir::toNativeSeparators(fontDirs.at(i)));
        fontsDir.setNameFilters(m_filters);
        const auto files = fontsDir.entryList(QDir::Files);
        for (const QString &fileName : files) {
            FontContext context(fontsDir.absolutePath(), fileName);

            auto it = m_sysfonts.find(fileName);
            if (it != m_sysfonts.end())
            {
                context.setSystem(it.value());
            }

            if (isFontSupportedByFreeType(context)) {
                m_names << context.name();
                m_families << context.family();
                m_systems << context.system();
                m_styles << context.style();
                m_paths << context.path();
            }
        }
        // qDebug() << __FILE__ << __LINE__ << m_names.size() << m_families.size() << m_systems.size() << m_styles.size() << m_paths.size();
        writeData();
    }

    resetLists();

    emit loadingFinished(m_total);
}

void FontManager::resetLists()
{
    m_names.clear();
    m_paths.clear();
    m_families.clear();
    m_styles.clear();
    m_systems.clear ();
}

bool FontManager::writeData()
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
        return false;
    QSqlQuery query(db);

    db.transaction();

    if(!query.prepare(QString(
                           "INSERT INTO %1 (name, path, family, style, system) "
                           "VALUES (:name, :path, :family, :style, :system) "
                           "ON CONFLICT(name, path) DO UPDATE SET "
                           "family = excluded.family, style = excluded.style, system = excluded.system "
                           "RETURNING id"
                           ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare " << query.lastQuery() << query.lastError();
        db.rollback();
        return false;
    }

    query.bindValue(":name", m_names);
    query.bindValue(":path", m_paths);
    query.bindValue(":family", m_families);
    query.bindValue(":style", m_styles);
    query.bindValue(":system", m_systems);

    if (!query.execBatch())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't append fonts" << query.lastError();
        db.rollback();
        return false;
    }

    // Обработать все результаты (важно для очистки)
    while (query.next()) {
        // Пропускаем результаты, если они не нужны
    }

    // Явно завершаем запрос
    query.finish();

    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError();
        return false;
    }

    QSqlQuery waitQuery(db);
    waitQuery.exec("SELECT 1");

    return true;
}

bool FontManager::appendOrUpdateFontContext(const QString & tableName, FontContext &context)
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
        return false;

    QSqlQuery query(db);

    db.transaction();

    if(!query.prepare(QString(
                           "INSERT INTO %1 (name, path, family, style, system) "
                           "VALUES (:name, :path, :family, :style, :system) "
                           "ON CONFLICT(name, path) DO UPDATE SET "
                           "family = excluded.family, style = excluded.style, system = excluded.system "
                           "RETURNING id"
                           ).arg(tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare " << query.lastQuery() << query.lastError();
        db.rollback();
        return false;
    }

    query.bindValue(":name", context.name());
    query.bindValue(":path", context.path());
    query.bindValue(":family", context.family());
    query.bindValue(":style", context.style());
    query.bindValue(":system", context.system());

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't append font" << context << query.lastError();
        db.rollback();
        return false;
    }

    while (query.next()) {
        // Пропускаем результаты, если они не нужны
    }

    // Явно завершаем запрос
    query.finish();

    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError();
        return false;
    }

    context.setId(query.lastInsertId().toInt());

    return true;
}

bool FontManager::isFontSupportedByFreeType(FontContext &context)
{
    FT_Library library;
    FT_Face face;

    if (FT_Init_FreeType(&library)) return false;

    QString filePath = context.path() + "/" + context.name();
    FT_Error ftError = FT_New_Face(
        library,
        filePath.toLocal8Bit().constData(),
        0,
        &face);

    // qDebug() << __FILE__ << __LINE__ << filePath << face->num_glyphs << face->num_charmaps << face->num_fixed_sizes;

    /**
     * TODO: Пока что не поддерживаются шрифты с фиксированным размером
     */
    if (!ftError && face->num_glyphs != 0 && face->num_fixed_sizes == 0)
    {
        context.setFamily(QString(face->family_name));
        context.setStyle(QString(face->style_name));
    }

    FT_Done_FreeType(library);

    return (ftError == 0);
}
