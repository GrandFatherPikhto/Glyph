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
        "bitmap_dimension INTEGER NOT NULL, "
        "glyph_size INTEGER, "
        "font_size INTEGER, "
        "padding_left INTEGER, "
        "padding_top INTEGER, "
        "padding_right INTEGER, "
        "padding_bottom INTEGER, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "UNIQUE(name, bitmap_dimension) ON CONFLICT REPLACE"
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
                          "(name, font_id, bitmap_dimension, glyph_size, font_size, padding_left, padding_top, padding_right, padding_bottom) "
                          "VALUES (:name, :font_id, :bitmap_dimension, :glyph_size, :font_size, :padding_left, :padding_top, :padding_right, :padding_bottom);").arg(m_tableName));

    query.bindValue(":name", profile.name());
    query.bindValue(":font_id", profile.fontId());
    query.bindValue(":bitmap_dimension", profile.bitmapDimension());
    query.bindValue(":glyph_size", profile.glyphSize());
    query.bindValue(":font_size", profile.fontSize());
    query.bindValue(":padding_left", profile.paddingLeft());
    query.bindValue(":padding_top", profile.paddingTop());
    query.bindValue(":padding_right", profile.paddingRight());
    query.bindValue(":padding_bottom", profile.paddingBottom());

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
        "SELECT id, name, font_id, bitmap_dimension, glyph_size, font_size, font_id, padding_left, padding_top, padding_right, padding_bottom FROM %1 WHERE name = :name AND font_id = :font_id").arg(m_tableName);

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
        qWarning() << __FILE__ << __LINE__ << "Can't find record with name = " << profile.name() << "AND font_id" << profile.fontId() << "; With Error:" << query.lastError();
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
    query.prepare("SELECT id, name, font_id, bitmap_dimension, glyph_size, font_size, font_id, padding_left, padding_top, padding_right, padding_bottom FROM profiles WHERE id = :id");
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
        profile.setBitmapDimension(query.value("bitmap_dimension").toInt());
        profile.setGlyphSize(query.value("glyph_size").toInt());
        profile.setFontSize(query.value("font_size").toInt());
        profile.setFontId(query.value("font_id").toInt());
        profile.setPaddingLeft(query.value("padding_left").toInt());
        profile.setPaddingTop(query.value("padding_top").toInt());
        profile.setPaddingRight(query.value("padding_right").toInt());
        profile.setPaddingBottom(query.value("padding_bottom").toInt());
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

    if (m_profile.bitmapDimension() < 0)
    {
        m_profile.setBitmapDimension(m_appSettings->value("defaultBitmapDimension").toInt());
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
    m_profile = settings.value("profile", ProfileContext()).value<ProfileContext>();
    settings.endGroup();

    defaultProfile(m_profile);
}

