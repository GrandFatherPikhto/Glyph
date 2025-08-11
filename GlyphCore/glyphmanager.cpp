#include "glyphmanager.h"
#include "appcontext.h"
#include "glyphcontext.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_tableName("glyphs")
    {
        QObject::connect(m_appContext, &AppContext::valuesInited, this, &GlyphManager::setupValues);
    }

GlyphManager::~GlyphManager()
{

}

void GlyphManager::setupValues()
{
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
            "size INTEGER NOT NULL, "      // uint8
            "offset_left INTEGER, "        // uint16
            "baseline INTERGER, "
            "bitmap_template BLOB, "
            "bitmap_preview BLOB, "
            "bitmap_draw BLOB, "
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

bool GlyphManager::findGlyph(const QChar &character, int profile, GlyphContext &glyph)
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
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph " << character << profile << ":" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't find glyph " << character << profile << ":" << query.lastError();
        return false;
    }

    // Заполняем структуру данными из запроса
    try {
        glyph.setGlyphId(query.value("id").toInt());
        glyph.setCharacter(query.value("character").toChar());
        glyph.setGlyphSize(query.value("size").toInt());
        glyph.setProfile(query.value("profile_id").toInt());
        glyph.setTemporary(query.value("temporary").toBool());
        glyph.setOffsetLeft(query.value("offset_left").toInt());
        glyph.setBaseline(query.value("baseline").toInt());
        // Доделать BLOBB поля
    }
    catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }


    return true;
}
