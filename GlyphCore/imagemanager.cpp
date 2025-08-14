#include "imagemanager.h"
#include "appsettings.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "profilemanager.h"

ImageManager::ImageManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
{
    connect(m_appContext, &AppContext::valuesInited, this, &ImageManager::initValues);
}

ImageManager::~ImageManager()
{

}

void ImageManager::initValues()
{
    Q_ASSERT(m_appContext->appSettings() != nullptr);
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();
    m_profileManager = m_appContext->profileManager();
}