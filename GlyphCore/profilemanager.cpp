#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFont>

#include "appcontext.h"
#include "profilemanager.h"
#include "fontmanager.h"
#include "gridmanager.h"
#include "appsettings.h"

#include "profilecontext.h"
#include "glyphcontext.h"

ProfileManager::ProfileManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_fontManager(appContext->fontManager())
    , m_gridManager(appContext->gridManager())
{
    // QObject::connect(m_appContext, &AppContext::valuesInited, this, &ProfileManager::setupValues);
    restoreSettings();
    setupValues ();
    setupSignals();
    createTable();
}

ProfileManager::~ProfileManager()
{
    saveSettings();
}

void ProfileManager::setupValues()
{
}

void ProfileManager::setupSignals ()
{
    QObject::connect(this, &ProfileManager::changeProfile, this, [=](const ProfileContext &profile){
        if (profile.isValid() && m_profile != profile)
        {
            m_profile = profile;
            emit profileChanged(m_profile);
        }
    });
}
bool ProfileManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ <<  "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    
    QString createTableQuery = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "font_id INTEGER, "
        "grid_id INTEGER, "
        "name VARCHAR(32) NOT NULL, "
        "glyph_size INTEGER, "
        "font_size INTEGER, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "UNIQUE(name, font_id, grid_id, glyph_size) ON CONFLICT REPLACE"
    ")").arg(m_tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
        
    // qDebug() << QString("Table %1 created successfully").arg(m_tableName);

    return true;
}

bool ProfileManager::appendProfile(ProfileContext &profile)
{
    if (!profile.isValid())
    {
        return false;
    }

    if (findProfile(profile))
    {
        return true;
    }
    
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    db.transaction();
    QSqlQuery query(db);

    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(name, font_id, grid_id, glyph_size, font_size) "
                          "VALUES (:name, :font_id, :grid_id, :glyph_size, :font_size);").arg(m_tableName));

    query.bindValue(":name", profile.name());
    query.bindValue(":font_id", profile.fontId());
    query.bindValue(":grid_id", profile.gridId());
    query.bindValue(":glyph_size", profile.glyphSize());
    query.bindValue(":font_size", profile.fontSize());

    if (!query.exec()) {
        qWarning() << __FILE__ << __LINE__ << "Failed to insert profile" << profile <<  query.lastError().text() << query.lastQuery();
        db.rollback();

        return false;
    }


    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError();
        return false;
    }

    profile.setId(query.lastInsertId().toInt());

    emit profilesChanged ();
    emit profileAppended (profile);

    return true;
}


bool ProfileManager::clearTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec(QString("DELETE FROM %1").arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Delete failed:" << query.lastError().text();
        return false;
    }

    emit profilesChanged ();

    return true;
}

const QString & ProfileManager::tableName() const 
{
    return m_tableName;
}

bool ProfileManager::findProfile(ProfileContext &profile)
{
    if (!profile.isValid())
        return false;

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    QString strSql = QString(
        "SELECT id, name, font_id, grid_id, glyph_size, font_size FROM %1 WHERE name = :name AND font_id = :font_id AND glyph_size = :glyph_size").arg(m_tableName);

    if(!query.prepare(strSql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL:" << query.lastQuery() << "; With Error:" << query.lastError();
        return false;
    }

    query.bindValue(":name", profile.name());
    query.bindValue(":font_id", profile.fontId());
    query.bindValue(":glyph_size", profile.glyphSize());

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Error execute SQL:" << query.lastQuery() << "; With Error:" << query.lastError();
        profile.setId(-1);
        return false;
    }

    if (!query.next())
    {
        // qWarning() << __FILE__ << __LINE__ << "Can't find record with name = " << profile.name() << "AND font_id" << profile.fontId() << "; With Error:" << query.lastError();
        profile.setId(-1);
        return false;
    }

    return assignQueryWithProfile(query, profile);
}


bool ProfileManager::getProfileById(int id, ProfileContext &profile)
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, name, font_id, grid_id, font_size, glyph_size, created_at FROM profiles WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't execute " << query.lastQuery() << ", Error: " << query.lastError();
        return false;
    }


    if (!query.next())
    {
        qWarning()  << __FILE__ << __LINE__ << "No profile found with id:" << id;
        return false;
    }

    return assignQueryWithProfile(query, profile);
}

bool ProfileManager::assignQueryWithProfile(QSqlQuery &query, ProfileContext &profile)
{
    // Заполняем структуру данными из запроса
    try {
        profile.setId(query.value("id").toInt());
        profile.setFontId(query.value("font_id").toInt());
        profile.setGridId(query.value("grid_id").toInt());
        profile.setName(query.value("name").toString());
        profile.setGlyphSize(query.value("glyph_size").toInt());
        profile.setFontSize(query.value("font_size").toInt());
    } catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}

void ProfileManager::defaultProfile(ProfileContext &profile)
{
    if (m_profile.glyphSize() < 0)
    {
        m_profile.setGlyphSize(m_appSettings->value("defaultGlyphSize").toInt());
    }

    if (m_profile.fontSize() < 0)
    {
        m_profile.setGlyphSize(m_appSettings->value("defaultFontSize").toInt());
    }
}

ProfileContext ProfileManager::defaultProfile()
{
    // qDebug() << __FILE__ << __LINE__ << m_appSettings->value("defaultProfileName").toString();
    return ProfileContext (
        -1
        , m_fontManager->fontContext().id()
        , m_gridManager->grid().id()
        , m_appSettings->value("defaultProfileName").toString()
        , m_appSettings->value("defaultGlyphSize").toInt()
        , m_appSettings->value("defaultFontSize").toInt()
    );
}

ProfileContext ProfileManager::glyphProfile(const GlyphContext &glyph)
{
    ProfileContext profile;

    if (glyph.profile() < 0)
    {
        return profile;
    }
    
    getProfileById(glyph.profile(), profile);
    
    return profile;
}

void ProfileManager::saveSettings()
{
    QSettings settings(this);
    settings.beginGroup("ProfileManager");
    settings.setValue("profile", m_profile);
    settings.endGroup();
}

void ProfileManager::restoreSettings()
{
    QSettings settings(this);
    settings.beginGroup("ProfileManager");
    m_profile = settings.value("profile", ProfileContext()).value<ProfileContext>();
    settings.endGroup();

    m_profile.setId(-1);
    defaultProfile(m_profile);
    m_profile.setGlyphSize(m_appSettings->value("defaultGlyphSize").toInt());
    // qDebug() << __FILE__ << __LINE__ << m_profile;
}
