#include <QSettings>

#include "glyphmanager.h"
#include "profilemanager.h"
#include "appcontext.h"
#include "glyphcontext.h"
#include "appsettings.h"
#include "fontmanager.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_profileManager(appContext->profileManager())
    , m_fontManager(appContext->fontManager())
    , m_tableName("glyphs")
{
    restoreSettings();
    setupValues();
    setupSignals();
}

GlyphManager::~GlyphManager()
{
    saveSettings ();
}

void GlyphManager::setupValues()
{
    createTable ();
}

void GlyphManager::setupSignals()
{
    QObject::connect(this, &GlyphManager::changeGlyph, this, [=](const GlyphContext &glyph){
        m_glyph = glyph;
        defaultGlyph(m_glyph);
        findGlyph(m_glyph);
        emit glyphChanged(m_glyph);
    });
}

bool GlyphManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
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
            "offset_left INTEGER, "        // uint16
            "baseline INTERGER, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
            "UNIQUE(unicode, profile_id) ON CONFLICT REPLACE"
            ");"
        ).arg(m_tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning() << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
    
    // Создаем индекс для ускорения поиска по категории (опционально)
    if (!query.exec(QString("CREATE INDEX IF NOT EXISTS idx_unicode ON %1(unicode)").arg(m_tableName))) {
        qWarning() << __FILE__ << __LINE__ << QString("Failed to create index %1: %2").arg(m_tableName, query.lastError().text());
    }
    
    return true;
}

bool GlyphManager::findGlyph(GlyphContext &context)
{
    if (context.profile() < 0 && m_profileManager->profile().id() < 0)
        return false;

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(QString("SELECT id, unicode, character, profile_id, glyph_size, offset_left, baseline FROM %1 WHERE unicode = :unicode AND profile_id = :profile").arg(m_tableName));

    query.bindValue(":unicode", QString::number(context.character().unicode()));
    query.bindValue(":profile", context.profile() < 0 ? m_profileManager->profile().id() : context.profile());

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

    // qDebug() << __FILE__ << __LINE__ << "Finded: " << context;

    return queryToContext(query, context);
}

bool GlyphManager::findGlyph(int id, GlyphContext &context)
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(
            QString(
                "SELECT id, unicode, character, profile_id, glyph_size, "
                " offset_left, baseline FROM %1 WHERE id = :id").arg(m_tableName)))
    {
        qWarning()  << __FILE__ << __LINE__ << "Can't prepare SQL" << query.lastQuery() << " with Error: " << query.lastError();
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

bool GlyphManager::removeGlyph(GlyphContext &context)
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    QString sql = QString("DELETE FROM %1 WHERE unicode = :unicode AND profile_id = :profile_id").arg(m_tableName);

    if(!query.prepare(sql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL" << query.lastQuery() << "with Error:" << query.lastError();
        return false;
    }

    query.bindValue(":unicode", context.character().unicode());
    query.bindValue(":profile_id", context.profile());

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL" << query.lastQuery() << "with Error:" << query.lastError();
        return false;
    }

    context.setId();
    context.setProfileId();

    emit glyphsTableChanged(m_profileManager->profile());
    emit glyphRemoved(context);

    return true;
}

bool GlyphManager::removeGlyphById(int id)
{
    GlyphContext context;

    if (!findGlyph(id, context))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    QString sql = QString("DELETE FROM %1 WHERE id = :id").arg(m_tableName);

    if(!query.prepare(sql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL" << query.lastQuery() << "with Error:" << query.lastError();
        return false;
    }

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL" << query.lastQuery() << "with Error:" << query.lastError();
        return false;
    }

    context.setId();
    context.setProfileId();

    emit glyphsTableChanged(m_profileManager->profile());
    emit glyphRemoved(context);

    return true;
}

bool GlyphManager::appendGlyphIfNotExists(GlyphContext &context)
{
    if (context.profile() < 0)
        return false;

    if (!context.isValid())
        return false;

    if(findGlyph(context))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    if(!query.prepare(QString(
        "INSERT INTO %1 (unicode, character, profile_id, glyph_size, offset_left, baseline) "
        "VALUES (:unicode, :character, :profile_id, :glyph_size, :offset_left, :baseline)").arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare " << query.lastQuery() << query.lastError();
        return false;
    }

    query.bindValue(":unicode", context.character().unicode());
    query.bindValue(":character", QString(context.character()));
    query.bindValue(":profile_id", context.profile());
    query.bindValue(":glyph_size", context.size());
    query.bindValue(":offset_left", context.offsetLeft());
    query.bindValue(":baseline", context.baseline());

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't append glyph" << context << query.lastError();
        return false;
    }

    context.setId(query.lastInsertId().toInt());

    emit glyphsTableChanged(m_profileManager->profile());
    emit glyphAppended(context);
    // qDebug() << __FILE__ << __LINE__ << context;

    return true;
}

bool GlyphManager::queryToContext(QSqlQuery &query, GlyphContext &context)
{
    // Заполняем структуру данными из запроса
    try {
        context.setId(query.value("id").toInt());
        QChar ch = query.value("unicode").toChar();
        context.setCharacter(ch);
        context.setSize(query.value("glyph_size").toInt());
        context.setProfileId(query.value("profile_id").toInt());
        context.setOffsetLeft(query.value("offset_left").toInt());
        context.setBaseline(query.value("baseline").toInt());
        // qDebug() << __FILE__ << __LINE__ << context << "Character: " << ch;
        // Доделать BLOBB поля
    } catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}

bool GlyphManager::queryGlyphsByProfile(QSqlQuery &query, const ProfileContext &profile)
{
    if(profile.id() < 0)
        return false;

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    query = QSqlQuery(db);

    QString sql = QString("SELECT id, unicode, character, profile_id, glyph_size, offset_left, baseline, created_at  FROM %1 WHERE profile_id = :profile_id ORDER BY unicode ASC").arg(m_tableName);
    if(!query.prepare(sql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL " << sql << ", Error:" << query.lastError();
        return false;
    }

    query.bindValue(":profile_id", profile.id());

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Error execute SQL " << sql << ", Error:" << query.lastError();
        return false;
    }

    return true;
}

void GlyphManager::defaultGlyph(GlyphContext &glyph)
{
    if (glyph.size() <= 0)
    {
        glyph.setSize(m_appSettings->value("defaultGlyphSize").toInt());
    }
}

void GlyphManager::saveSettings()
{
    QSettings settings(this);
    settings.beginGroup("GlyphManager");
    settings.setValue("glyph", m_glyph);
    settings.endGroup();
}

void GlyphManager::restoreSettings()
{
    QSettings settings(this);
    settings.beginGroup("GlyphManager");
    m_glyph = settings.value("glyph", GlyphContext()).value<GlyphContext>();
    settings.endGroup();
    defaultGlyph(m_glyph);
}
