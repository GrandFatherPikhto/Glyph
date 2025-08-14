#include "appcontext.h"
#include "fontmanager.h"
#include "charmapmanager.h"
#include "unicodemetadata.h"
#include "profilemanager.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "imagemanager.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_appSettings(nullptr)
    , m_fontManager(nullptr)
    , m_charmapManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_profileManager(nullptr)
    , m_glyphManager(nullptr)
    , m_imageManager(nullptr)
{
    initDatabase();
    setupVariables();
}

AppContext::~AppContext()
{
    // qDebug() << __FILE__ << __LINE__ << "Destroy AppContext class";
    releaseDatabase ();
}

void AppContext::setupVariables()
{
}

AppSettings* AppContext::appSettings()
{
    if (m_appSettings == nullptr)
    {
        m_appSettings = new AppSettings(this);
    }

    return m_appSettings; 
}

FontManager * AppContext::fontManager()
{ 
    if (m_fontManager == nullptr)
    {
        m_fontManager = new FontManager(this);
    }

    return m_fontManager; 
}

CharmapManager * AppContext::charmapManager()
{
    if (m_charmapManager == nullptr)
    {
        m_charmapManager = new CharmapManager(this);
    }

    return m_charmapManager;
}

UnicodeMetadata * AppContext::unicodeMetadata()
{ 
    if (m_unicodeMetadata == nullptr)
    {
        m_unicodeMetadata = new UnicodeMetadata(this);
    }

    return m_unicodeMetadata;
}

ProfileManager * AppContext::profileManager()
{
    if (m_profileManager == nullptr)
    {
        m_profileManager = new ProfileManager(this);
    }

    return m_profileManager;
}

GlyphManager * AppContext::glyphManager()
{
    if (m_glyphManager == nullptr)
    {
        m_glyphManager = new GlyphManager(this);
    }

    return m_glyphManager;
}

ImageManager * AppContext::imageManager()
{
    if (m_imageManager == nullptr)
    {
        m_imageManager = new ImageManager(this);
    }

    return m_imageManager;
}

QString AppContext::appDataPath()
{
    // Получение пути к AppData/Local/<AppName>
    QString appDataPath = appSettings()->appDataPath();

    // Пример: создание файла БД в этом каталоге
    QString dbPath = appDataPath + "/database.sqlite";

    return dbPath;
}

bool AppContext::initDatabase()
{
    releaseDatabase();

    QSqlDatabase db = QSqlDatabase::database();

    // Подключаемся к базе данных SQLite (файл будет создан автоматически)
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(appDataPath());

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    return true;
}

bool AppContext::releaseDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(db.isOpen())
    {
        db.close();
        return true;
    }

    return false;
}
