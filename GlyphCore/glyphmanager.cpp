#include "glyphmanager.h"
#include "appcontext.h"
#include "glyphcontext.h"
#include "appsettings.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(nullptr)
    , m_tableName("glyphs")
{
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &GlyphManager::setupValues);
}

GlyphManager::~GlyphManager()
{

}

void GlyphManager::setupValues()
{
    Q_ASSERT(m_appContext->appSettings() != nullptr);

    m_appSettings = m_appContext->appSettings();

    setupSignals();
    createTable ();
}

void GlyphManager::setupSignals()
{

}

bool GlyphManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    QString createTableQuery =
        QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "unicode INTEGER NOT NULL, "   // uint32
            "character TEXT, "
            "profile_id INTEGER NOT NULL, "
            "glyph_size INTEGER NOT NULL, "      // uint8
            "temporary BOOL DEFAULT TRUE, "
            "offset_left INTEGER, "        // uint16
            "baseline INTERGER, "
            "bitmap_template BLOB DEFAULT NULL, "
            "bitmap_preview BLOB DEFAULT NULL, "
            "bitmap_draw BLOB DEFAULT NULL, "
            // "KEY (unicode, profile_id) "        // Добавляем первичный ключ
            "UNIQUE(unicode, profile_id) ON CONFLICT REPLACE"
            ");"
        ).arg(m_tableName);
    
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

#if 0
bool GlyphManager::findGlyph(const QChar &character, int profile, GlyphContext &context)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(QString("SELECT id, unicode, character, profile_id, size, offset_left, baseline, temporary, bitmap_template, bitmap_preview, bitmap_draw FROM %1 WHERE character = :character AND profile_id = :profile").arg(m_tableName));

    query.bindValue(":character", QString(character));
    query.bindValue(":profile", profile);

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << character << profile << ":" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << character << profile << ":" << query.lastError();
        return false;
    }

    return queryToContext(query, context);
}
#endif

bool GlyphManager::findGlyph(GlyphContext &context)
{
    if (context.profile() < 0)
        return false;

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(QString("SELECT id, unicode, character, profile_id, glyph_size, temporary, offset_left, baseline, bitmap_template, bitmap_preview, bitmap_draw FROM %1 WHERE character = :character AND profile_id = :profile").arg(m_tableName));

    query.bindValue(":character", QString(context.character()));
    query.bindValue(":profile", context.profile());

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << context << ":" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        // qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << context << ":" << query.lastError();
        return false;
    }

    if (findGlyph(context))
    {
        return false;
    }



    return queryToContext(query, context);
}

bool GlyphManager::findGlyph(int id, GlyphContext &context)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(
            QString(
                "SELECT id, unicode, character, profile_id, glyph_size, "
                " offset_left, baseline, temporary, bitmap_template, "
                "bitmap_preview, bitmap_draw FROM %1 WHERE id = :id").arg(m_tableName)))
    {
        qWarning() << "Can't prepare SQL" << query.lastQuery() << " with Error: " << query.lastError();
        return false;
    }

    query.bindValue(":id", QString::number(id));

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << context << ":" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph context " << context << ":" << query.lastError();
        return false;
    }

    if (findGlyph(context))
    {
        return false;
    }

    return queryToContext(query, context);
}

bool GlyphManager::appendGlyphIfNotExists(GlyphContext &context)
{
    GlyphProfile profile = m_appSettings->glyphProfile();
    qDebug() << __FILE__ << __LINE__ << profile;

    if (profile.id() < 0)
        return false;

    if (context.profile() < 0)
    {
        context.setProfile(profile.id());
    }

    if (context.glyphSize() < 0 && profile.glyphSize() > 0)
    {
        context.setGlyphSize(profile.glyphSize());
    }

    qDebug() << __FILE__ << __LINE__ << context;


    if(findGlyph(context))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    qDebug() << __FILE__ << __LINE__ << context;

    QSqlQuery query(db);
    if(!query.prepare(QString(
        "INSERT INTO %1 (unicode, character, profile_id, glyph_size, temporary, offset_left, baseline) "
        "VALUES (:unicode, :character, :profile_id, :glyph_size, :temporary, :offset_left, :baseline)").arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare " << query.lastQuery() << query.lastError();
        return false;
    }

    query.bindValue(":unicode", context.character().unicode());
    query.bindValue(":character", QString(context.character()));
    query.bindValue(":profile_id", context.profile());
    query.bindValue(":glyph_size", context.glyphSize());
    query.bindValue(":temporary", context.temporary());
    query.bindValue(":offset_left", context.offsetLeft());
    query.bindValue(":baseline", context.baseline());

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't append glyph" << context << query.lastError();
        return false;
    }

    return true;
}

bool GlyphManager::queryToContext(QSqlQuery &query, GlyphContext &context)
{
    // Заполняем структуру данными из запроса
    try {
        context.setGlyphId(query.value("id").toInt());
        context.setCharacter(query.value("character").toChar());
        context.setGlyphSize(query.value("glyph_size").toInt());
        context.setProfile(query.value("profile_id").toInt());
        context.setTemporary(query.value("temporary").toBool());
        context.setOffsetLeft(query.value("offset_left").toInt());
        context.setBaseline(query.value("baseline").toInt());
        // Доделать BLOBB поля
    } catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}
