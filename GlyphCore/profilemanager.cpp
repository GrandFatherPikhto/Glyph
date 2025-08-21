#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFont>

#include "profilemanager.h"
#include "appcontext.h"
#include "appsettings.h"
#include "profilecontext.h"
#include "glyphcontext.h"
#include "fontmanager.h"

ProfileManager::ProfileManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_fontManager(appContext->fontManager())
    , m_appSettings(appContext->appSettings())
    , m_tableName("profiles")
{
    // QObject::connect(m_appContext, &AppContext::valuesInited, this, &ProfileManager::setupValues);
    setupValues ();
}

ProfileManager::~ProfileManager()
{
    saveSettings();
}

void ProfileManager::setupValues()
{
    Q_ASSERT(m_appContext->fontManager() != nullptr);
    m_fontManager = m_appContext->fontManager();
    restoreSettings();
    setupSignals();
    createTable();
}

void ProfileManager::setupSignals ()
{
    QObject::connect(this, &ProfileManager::changeProfile, this, [=](const ProfileContext &profile){
        m_profile = profile;
        emit profileChanged(m_profile);
    });
}


GlyphContext ProfileManager::defaultGlyphContext(const QChar &ch)
{
    if(ch == QChar())
        return GlyphContext();

    GlyphContext context;
    context.setCharacter(ch);
    context.setProfileId(m_profile.id());
    context.setSize(m_profile.glyphSize());

    return context;
}

bool ProfileManager::defaultGlyphContext(GlyphContext &context)
{
    if(context.character() == QChar())
        return false;

    if (context.profile() < 0)
        context.setProfileId(m_profile.id());

    if (context.size() < 0)
        context.setSize(m_profile.glyphSize());

    return true;
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
        "name TEXT NOT NULL, "
        "glyph_size INTEGER, "
        "font_size INTEGER, "
        "grid_width INTEGER, "
        "grid_height INTEGER, "
        "grid_left INTEGER, "
        "grid_top INTEGER, "
        "grid_right INTEGER, "
        "grid_bottom INTEGER, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "UNIQUE(name, font_id, glyph_size) ON CONFLICT REPLACE"
    ")").arg(m_tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
        
    // qDebug() << QString("Table %1 created successfully").arg(m_tableName);

    return true;
}

bool ProfileManager::insertOrReplaceProfile(const ProfileContext &profile)
{
    if (!profile.isValid())
    {
        return false;
    }
    
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    qDebug() << __FILE__ << __LINE__ << profile;

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(name, font_id, glyph_size, font_size, grid_width, grid_height, grid_left, grid_top, grid_right, grid_bottom) "
                          "VALUES (:name, :font_id, :glyph_size, :font_size, :grid_width, :grid_height, :grid_left, :grid_top, :grid_right, :grid_bottom);").arg(m_tableName));

    query.bindValue(":name", profile.name());
    query.bindValue(":font_id", profile.fontId());
    query.bindValue(":glyph_size", profile.glyphSize());
    query.bindValue(":font_size", profile.fontSize());
    query.bindValue(":grid_width", profile.gridWidth());
    query.bindValue(":height", profile.gridHeight());
    query.bindValue(":grid_left", profile.gridLeft());
    query.bindValue(":grid_top", profile.gridTop());
    query.bindValue(":grid_right", profile.gridRight());
    query.bindValue(":grid_bottom", profile.gridBottom());

    if (!query.exec()) {
        qWarning() << __FILE__ << __LINE__ << "Failed to insert script" <<  query.lastError().text() << query.lastQuery();
        db.rollback();

        return false;
    }


    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError().text();
        return false;
    }

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
        "SELECT id, name, font_id, glyph_size, font_size, font_id, grid_width, grid_height, grid_left, grid_top, grid_right, grid_bottom FROM %1 WHERE name = :name AND font_id = :font_id").arg(m_tableName);

    if(!query.prepare(strSql))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare SQL:" << query.lastQuery() << "; With Error:" << query.lastError();
        return false;
    }

    query.bindValue(":name", profile.name());
    query.bindValue(":font_id", profile.fontId());

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

    return assignQueryWithProfile(profile, std::move(query));
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
    query.prepare("SELECT id, name, font_id, glyph_size, font_size, font_id, grid_width, grid_height, grid_left, grid_top, grid_right, grid_bottom FROM profiles WHERE id = :id");
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

    return assignQueryWithProfile(profile, std::move(query));
}

bool ProfileManager::assignQueryWithProfile(ProfileContext &profile, QSqlQuery query)
{
    // Заполняем структуру данными из запроса
    try {
        profile.setId(query.value("id").toInt());
        profile.setName(query.value("name").toString());
        profile.setGlyphSize(query.value("glyph_size").toInt());
        profile.setFontSize(query.value("font_size").toInt());
        profile.setFontId(query.value("font_id").toInt());
        profile.setGridWidth(query.value("grid_width").toInt());
        profile.setGridHeight(query.value("grid_height").toInt());
        profile.setGridLeft(query.value("grid_left").toInt());
        profile.setGridTop(query.value("grid_top").toInt());
        profile.setGridRight(query.value("grid_right").toInt());
        profile.setGridBottom(query.value("grid_bottom").toInt());
    }
    catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}

bool ProfileManager::defaultProfile(ProfileContext &profile)
{
    bool res = false;
    if (profile.fontId() < 0)
    {
        profile.setFontId(m_fontManager->fontContext().id());
        res = true;
    }

    if (m_profile.glyphSize() < 0)
    {
        m_profile.setGlyphSize(m_appSettings->value("defaultGlyphSize").toInt());
        res = true;
    }

    if (m_profile.gridWidth() < 0)
    {
        m_profile.setGridWidth(m_appSettings->value("defaultGridWidth").toInt());
        res = true;
    }

    if (m_profile.gridHeight() < 0)
    {
        m_profile.setGridHeight(m_appSettings->value("defaultGridHeight").toInt());
        res = true;
    }

    if (m_profile.gridLeft() < 0)
    {
        m_profile.setGridLeft(m_appSettings->value("defaultGridLeft").toInt());
        res = true;
    }

    if (m_profile.gridTop() < 0)
    {
        m_profile.setGridTop(m_appSettings->value("defaultGridTop").toInt());
        res = true;
    }

    if (m_profile.gridRight() < 0)
    {
        m_profile.setGridRight(m_appSettings->value("defaultGridRight").toInt());
        res = true;
    }

    if (m_profile.gridBottom() < 0)
    {
        m_profile.setGridBottom(m_appSettings->value("defaultGridBottom").toInt());
        res = true;
    }

    if(findProfile(profile))
    {
        res = true;
    } else
    {
        profile.setId(-1);
    }

    return res;
}

ProfileContext ProfileManager::glyphProfile(const GlyphContext &glyph)
{
    if (glyph.profile() < 0)
    {
        return m_profile;
    }
    
    ProfileContext profile;
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
    // m_profile = settings.value("profile", ProfileContext()).value<ProfileContext>();    
    settings.endGroup();

    m_profile.setId(-1);
    m_profile.setGlyphSize(m_appSettings->value("defaultGlyphSize").toInt());
    m_profile.setFontSize(m_appSettings->value("defaultFontSize").toInt());
    m_profile.setGridWidth(m_appSettings->value("defaultGridWidth").toInt());
    m_profile.setGridHeight(m_appSettings->value("defaultGridHeight").toInt());
    m_profile.setGridLeft(m_appSettings->value("defaultGridLeft").toInt());
    m_profile.setGridTop(m_appSettings->value("defaultGridTop").toInt());
    m_profile.setGridRight(m_appSettings->value("defaultGridRight").toInt());
    m_profile.setGridBottom(m_appSettings->value("defaultGridBottom").toInt());

    // qDebug() << __FILE__ << __LINE__ << m_profile;

    defaultProfile(m_profile);
}

