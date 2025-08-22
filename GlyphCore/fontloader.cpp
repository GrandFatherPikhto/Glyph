#include <QDir>

#include "fontloader.h"
#include "fontutils.h"

FontLoader::FontLoader(const QString &tableName, const QString &dbFile, QObject *parent)
    : QObject{parent}
    , m_tableName(tableName)
    , m_dbFile(dbFile)
    , m_counter(0)
    , m_total(0)
    , m_filters({"*.ttf", "*.ttc", "*.otf", "*.otc"
                , "*.pfa", "*.pfb", "*.woff", "*.woff2"
                , "*.fon", "*.fnt", "*.bdf", "*.pcf"})
{
    ::getWinSystemFonts(m_sysfonts);
    ::getWinSystemFonts(m_sysfonts, HKEY_CURRENT_USER);
    connect(this, &FontLoader::startLoading, this, [=](const QStringList &list){
        loadFonts(list);
    });
}

FontLoader::~FontLoader()
{

}

void FontLoader::setFilter(QDir &dir)
{

}

int FontLoader::totalCounter(const QStringList &list)
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

void FontLoader::loadFonts(const QStringList &fontDirs)
{
    m_counter = 0;
    m_total = 0;
    resetLists();
    QString connectionName = QString("thread_%1").arg(GetCurrentThreadId());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    if (!db.open())
    {
        qWarning() << "Can't open DB in worker thread" << m_dbFile;
        QSqlDatabase::removeDatabase(connectionName); // Важно!
        return;
    }

    db.setDatabaseName(m_dbFile);
    if (!db.open())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't open DB" << m_dbFile;
        return;
    }

    // m_total = totalCounter(fontDirs);
    emit start(m_total);

    for (int i = 0; i < fontDirs.size(); ++i) {
        m_total += processFontDir(db, fontDirs.at(i));
    }

    db.close();
    QSqlDatabase::removeDatabase(connectionName); 

    resetLists();

    emit finished(m_total);
}

int FontLoader::processFontDir(QSqlDatabase db, const QString &path)
{
    int counter = 0;
    QDir fontsDir(QDir::toNativeSeparators(path));
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
    writeData(db);

    return m_paths.size();
}

void FontLoader::resetLists()
{
    m_names.clear();
    m_paths.clear();
    m_families.clear();
    m_styles.clear();
    m_systems.clear ();
}

bool FontLoader::writeData(QSqlDatabase db)
{
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

bool FontLoader::appendOrUpdateFontContext(QSqlDatabase db, const QString & tableName, FontContext &context)
{
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

bool FontLoader::isFontSupportedByFreeType(FontContext &context)
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
