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
    , m_fontManager(nullptr)
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


GlyphContext ProfileManager::defaultGlyphContext(const QChar &ch, bool temporary)
{
    if(ch == QChar())
        return GlyphContext();

    GlyphContext context;
    context.setCharacter(ch);
    context.setProfileId(m_profile.id());
    context.setSize(m_profile.glyphSize());
    context.setTemporary(temporary);

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
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    
    QString createTableQuery = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "font_family TEXT, "
        "font_path TEXT, "
        "bitmap_dimension INTEGER NOT NULL, "
        "glyph_size INTEGER NOT NULL, "
        "temporary BOOL, "
        "padding_left INTEGER, "
        "padding_top INTEGER, "
        "padding_right INTEGER, "
        "padding_bottom INTEGER, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "UNIQUE(name, bitmap_dimension) ON CONFLICT REPLACE"
    ")").arg(m_tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning() << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
        
    // qDebug() << QString("Table %1 created successfully").arg(m_tableName);

    return true;
}

bool ProfileManager::insertOrReplaceProfile(const ProfileContext &profile)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    qDebug() << __FILE__ << __LINE__ << profile;

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(name, font_family, font_path, bitmap_dimension, glyph_size, temporary, padding_left, padding_top, padding_right, padding_bottom) "
                          "VALUES (:name, :font_family, :font_path, :bitmap_dimension, :glyph_size, :temporary, :padding_left, :padding_top, :padding_right, :padding_bottom);").arg(m_tableName));

    query.bindValue(":name", profile.name());
    query.bindValue(":font_family", profile.font().family());
    query.bindValue(":font_path", profile.fontPath());
    query.bindValue(":bitmap_dimension", profile.bitmapDimension());
    query.bindValue(":glyph_size", profile.glyphSize());
    query.bindValue(":temporary", profile.temporary());
    query.bindValue(":padding_left", profile.paddingLeft());
    query.bindValue(":padding_top", profile.paddingTop());
    query.bindValue(":padding_right", profile.paddingRight());
    query.bindValue(":padding_bottom", profile.paddingBottom());

    if (!query.exec()) {
        qWarning() << "Failed to insert script" <<  query.lastError().text() << query.lastQuery();
        db.rollback();

        return false;
    }


    if (!db.commit()) {
        qWarning() << "Commit failed:" << db.lastError().text();
        return false;
    }

    emit profilesChanged ();

    return true;
}



bool ProfileManager::clearTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec(QString("DELETE FROM %1").arg(m_tableName)))
    {
        qWarning() << "Delete failed:" << query.lastError().text();
        return false;
    }

    emit profilesChanged ();

    return true;
}

const QString & ProfileManager::tableName() const 
{
    return m_tableName;
}

bool ProfileManager::getProfileById(int id, ProfileContext &profile)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, name, bitmap_dimension, glyph_size, font_family, font_path, temporary, padding_left, padding_top, padding_right, padding_bottom FROM profiles WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.exec())
    {
        qWarning() << "Can't execute " << query.lastQuery() << ", Error: " << query.lastError();
        return false;
    }


    if (!query.next())
    {
        qWarning() << "No profile found with id:" << id;
        return false;
    }

    // Заполняем структуру данными из запроса
    try {
        profile.setId(query.value("id").toInt());
        profile.setName(query.value("name").toString());
        profile.setBitmapDimension(query.value("bitmap_dimension").toInt());
        profile.setGlyphSize(query.value("glyph_size").toInt());

        QFont font(query.value("font_family").toString());
        profile.setFont(font);
        profile.setFontPath(query.value("font_path").toString());
        profile.setTemporary(query.value("temporary").toBool());
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

    // qDebug() << __FILE__ << __LINE__ << m_profile << m_fontManager->loadFont(m_profile.font());
    if (m_profile.font() != QFont() && m_profile.fontPath().isEmpty() && m_fontManager->loadFont(m_profile.font()))
    {
        m_profile.setFontPath(m_fontManager->fontPath());
    }

    settings.endGroup();
}
