#include "imagemanager.h"
#include "appsettings.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "profilemanager.h"

ImageManager::ImageManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
{
    initValues ();
}

ImageManager::~ImageManager()
{

}

void ImageManager::initValues()
{
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();
    m_profileManager = m_appContext->profileManager();
}