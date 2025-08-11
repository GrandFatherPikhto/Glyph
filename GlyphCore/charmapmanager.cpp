#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QPair>

#include "charmapmanager.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "dbcore.h"
#include "sqlfilter.h"

CharmapManager::CharmapManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_fontSize(12)
    , m_fontPath(QString())
    , m_fontFamily(QString())
    , m_font(QFont())
    , m_tableName("charmap")
{
    // m_sqlFilter = new SqlFilter(this);
    // m_sqlBegin = QString(
    //                  "SELECT c.unicode, c.character, cd.name AS category, "
    //                  "sd.name AS script, dd.name AS decomposition "
    //                  "FROM %1 c "
    //                  "JOIN category_data cd ON c.category = cd.id "
    //                  "JOIN script_data sd ON c.script = sd.id "
    //                  "JOIN decomposition_data dd ON c.decomposition = dd.id "
    //                  ).arg(m_tableName);

    QObject::connect(m_appContext, &AppContext::valuesInited, this, &CharmapManager::setupValues);
}

CharmapManager::~CharmapManager()
{
    releaseFontFace();
    releaseFtLibrary ();
}

void CharmapManager::setupValues ()
{
    Q_ASSERT(m_appContext->fontManager() != nullptr);

    m_filter = new SqlFilter(this);

    m_filter->addCondition("category", QSharedPointer<Condition>::create("cd.id", "cd_id"));
    m_filter->addCondition("script", QSharedPointer<Condition>::create("sd.id", "sd_id"));
    m_filter->addCondition("decomposition", QSharedPointer<Condition>::create("dd.id", "dd_id"));
    m_filter->addCondition("msb", QSharedPointer<Condition>::create("(c.unicode >> 8)", "msb"));
    m_filter->addCondition("character", QSharedPointer<Condition>::create("c.unicode", "cu"));

    m_fontManager = m_appContext->fontManager();

    initFtLibrary();

    createCharacterTable();

    setupSignals ();
}

void CharmapManager::setupSignals ()
{
    
}

void CharmapManager::initFtLibrary()
{
    releaseFtLibrary();
    if (FT_Init_FreeType(&m_ftLibrary)) {
        qWarning() << __FILE__ << __LINE__ << "Could not init FreeType library";
    }
}

bool CharmapManager::loadFontFace()
{
    releaseFontFace ();

    FT_Error ftError = FT_New_Face(m_ftLibrary, m_fontPath.toStdString().c_str(), 0, &m_ftFace);
    // Загрузка шрифта
    if (ftError) {
        QString strError(FT_Error_String(ftError));
        qWarning() << __FILE__ << __LINE__ << "Could not open font" << m_fontPath << ftError << strError;
        releaseFontFace();
        return false;
    }

    return true;
}

bool CharmapManager::loadFont(const QFont &font)
{
    if (m_font == font)
        return false;

    m_font = font;
    if(m_fontManager->loadFont(m_font))
    {
        m_fontPath = m_fontManager->fontPath();
        // qDebug() << __FILE__ << __LINE__ << m_fontPath;
        m_fontFamily = m_font.family();
        if (loadFontFace())
        {
            loadCharacterTable();
        }

        return false;
    }

    return true;
}

bool CharmapManager::loadCharacterTable()
{
    if (m_fontPath.isEmpty() || m_fontFamily.isEmpty() || !m_ftFace)
        return false;

    FT_Set_Pixel_Sizes(m_ftFace, 0, m_fontSize);

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QString("DELETE FROM %1").arg(m_tableName));

    if (!query.exec())
    {
        qWarning() << "Delete failed:" << query.lastError().text();
        return false;
    }

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                  "(unicode, character, category, script, decomposition) "
                  "VALUES (:unicode, :character, :category, :script, :decomposition)").arg(m_tableName));

    FT_ULong charcode;
    FT_UInt gindex;
    charcode = FT_Get_First_Char(m_ftFace, &gindex);

    while (gindex != 0)
    {
        if (charcode && charcode < 0xFFFF)
        {
            QChar ch(static_cast<quint32>(charcode));

            // Получаем правильные значения для каждого поля
            uint8_t category = static_cast<uint8_t>(ch.category()); // QChar::Category -> uint8
            uint16_t script = static_cast<uint16_t>(ch.script());   // QChar::Script -> uint16
            uint16_t decomposition = ch.decompositionTag(); // Заглушка - в Qt нет direct decompositionTag()


            // Привязываем значения
            query.bindValue(":unicode", static_cast<quint32>(ch.unicode()));
            query.bindValue(":character", QString(ch));
            query.bindValue(":category", category);
            query.bindValue(":script", script);
            query.bindValue(":decomposition", decomposition);

            // if (!query.isActive())
            // {
            //     qWarning() << __FILE__ << __LINE__ << query.lastError();
            // }

            if (!query.exec()) {
                qWarning() << "Failed to insert character" << static_cast<quint32>(ch.unicode()) << ":" << query.lastError().text() << query.lastQuery();
                db.rollback();

                return false;
            }
        }

        charcode = FT_Get_Next_Char(m_ftFace, charcode, &gindex);
    }

    if (!db.commit()) {
        qWarning() << "Commit failed:" << db.lastError().text();
        return false;
    }

    emit charmapUpdated(m_font);

    return true;
}

bool CharmapManager::createCharacterTable()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    
    QString createTableQuery = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "unicode INTEGER NOT NULL, "   // uint32
        "character TEXT, "
        "category INTEGER, "           // uint8
        "script INTEGER, "             // uint16
        "decomposition INTEGER, "      // uint8
        "PRIMARY KEY (unicode)"        // Добавляем первичный ключ
        ")").arg(m_tableName);
    

    if (!query.exec(createTableQuery)) {
        qWarning() << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
    
    // Создаем индекс для ускорения поиска по категории (опционально)
    if (!query.exec(QString("CREATE INDEX IF NOT EXISTS idx_category ON %1(category)").arg(m_tableName))) {
        qWarning() << QString("Failed to create index %1: %2").arg(m_tableName, query.lastError().text());
        // Не считаем это критической ошибкой
    }
    
    // qDebug() << QString("Table %1 created successfully").arg(m_tableName);
    return true;
}

void CharmapManager::releaseFtLibrary()
{
    if (m_ftLibrary != 0)
    {
        FT_Done_Library(m_ftLibrary);
        m_ftLibrary = 0;
    }
}

void CharmapManager::releaseFontFace()
{
    if (m_ftFace != 0)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

bool CharmapManager::execQuery(QSqlQuery &query, const QString &sqlSort)
{    
    QString sql = QString(
        "SELECT c.unicode, c.character, cd.name AS category, "
        "sd.name AS script, dd.name AS decomposition "
        "FROM %1 c "
        "JOIN category_data cd ON c.category = cd.id "
        "JOIN script_data sd ON c.script = sd.id "
        "JOIN decomposition_data dd ON c.decomposition = dd.id "
    ).arg(m_tableName);

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    query = QSqlQuery(db);

    QString filter = m_filter->prepare();
    if(!filter.isEmpty())
    {
        sql += " " + filter;
    }

    sql += " " + sqlSort;

    // qDebug() << __FILE__ << __LINE__ << sql;

    if (!query.prepare(sql))
    {
        qWarning() << "Can't prepare sql query " << sql << ", Error: " << query.lastError();
        return false;
    }

    const QList<QPair<QString, QVariant>> &binds = m_filter->binds();

    for (auto const &bind : binds)
    {
        query.bindValue(bind.first, bind.second);
    }

    if (!query.exec())
    {
        qWarning() << "Can't execute sql query " << sql << query.lastQuery();
    }

    return true;
}
