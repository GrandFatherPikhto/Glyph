#include <QStandardPaths>
#include <QDir>

#include "appdata.h"
#include "appcontext.h"

AppData::AppData(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appDataPath(QString())
{
    initAppDataCatalog();
}

AppData::~AppData()
{

}

void AppData::initAppDataCatalog()
{
    // Получение пути к AppData/Local/<AppName>
    m_appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    // Создание каталога, если его нет
    QDir dir(m_appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}
