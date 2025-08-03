#include "appcontext.h"
#include "appproject.h"
#include "dbcore.h"
#include "appsettings.h"

#include <QDir>
#include <QFileInfo>

AppProject::AppProject(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
{}

AppProject::~AppProject()
{

}


bool AppProject::createProject(const QString &pathName)
{
    QDir projectDir(pathName);
    if (projectDir.exists())
        return false;

    QString projectName(projectDir.dirName());

    projectDir.cdUp();

    if(!projectDir.exists())
    {
        return false;
    }

    m_projectPath = pathName;
    m_projectName = projectName;

    m_appSettings = m_appContext->appSettings();

    m_appSettings->setDefaultProjectPath(projectDir.path());

    projectDir.mkdir(pathName);

    m_dbCore = m_appContext->dbCore();

    m_dbCore->createDatabase(m_projectPath);

    return true;
}
