#include <QSettings>

#include "glyphmanager.h"
#include "profilemanager.h"
#include "appcontext.h"
#include "glyphcontext.h"
#include "appsettings.h"

GlyphManager::GlyphManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(nullptr)
    , m_profileManager(nullptr)
    , m_tableName("glyphs")
{
    restoreSettings();
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &GlyphManager::setupValues);
}

GlyphManager::~GlyphManager()
{
    saveSettings ();
}

void GlyphManager::setupValues()
{
    Q_ASSERT(m_appContext->appSettings() != nullptr && m_appContext->profileManager() != nullptr);

    m_appSettings = m_appContext->appSettings();
    m_profileManager = m_appContext->profileManager();

    setupSignals();
    createTable ();
}

void GlyphManager::setupSignals()
{
    QObject::connect(this, &GlyphManager::changeGlyph, this, [=](const GlyphContext &glyph){
        m_glyph = glyph;
        emit glyphChanged(m_glyph);
    });
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
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
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

    query.prepare(QString("SELECT id, unicode, character, profile_id, glyph_size, temporary, offset_left, baseline, bitmap_template, bitmap_preview, bitmap_draw FROM %1 WHERE unicode = :unicode AND profile_id = :profile").arg(m_tableName));

    query.bindValue(":unicode", QString::number(context.character().unicode()));
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

    // qDebug() << __FILE__ << __LINE__ << "Finded: " << context;

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

bool GlyphManager::removeGlyph(GlyphContext &context)
{
    if (context.id() < 0)
    {
        m_profileManager->defaultGlyphContext(context);

        if(findGlyph(context))
        {
            removeGlyphById(context.id());
            return true;
        }
    } else
    {
        qDebug() << __FILE__ << __LINE__ << context;
        removeGlyphById(context.id());
        return true;
    }

    return false;
}

bool GlyphManager::removeGlyphById(int id)
{
    GlyphContext context;
    if (!findGlyph(id, context))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
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

    context.resetId();
    context.resetProfile();

    emit glyphsTableChanged(m_profileManager->profile());
    emit glyphRemoved(context);

    return true;
}

bool GlyphManager::appendGlyphIfNotExists(GlyphContext &context)
{
    m_profileManager->defaultGlyphContext(context);

    if(findGlyph(context))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    // qDebug() << __FILE__ << __LINE__ << context;

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
    query.bindValue(":glyph_size", context.size());
    query.bindValue(":temporary", context.temporary());
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
        context.setTemporary(query.value("temporary").toBool());
        context.setOffsetLeft(query.value("offset_left").toInt());
        context.setBaseline(query.value("baseline").toInt());
        context.setTemporary();
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

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    query = QSqlQuery(db);

    QString sql = QString("SELECT id, unicode, character, profile_id, glyph_size, temporary, offset_left, baseline, bitmap_template, bitmap_preview, bitmap_draw, created_at  FROM %1 WHERE profile_id = :profile_id ORDER BY unicode ASC").arg(m_tableName);
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
}
