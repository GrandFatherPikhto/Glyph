#include "profilemanager.h"
#include "glyphkey.h"
#include "glyphprofile.h"
#include "appcontext.h"

ProfileManager::ProfileManager(AppContext *appContext) 
    : QObject{appContext}
    , m_appContext(appContext)
    , m_profile(QSharedPointer<GlyphProfile>())
{
    m_profiles = std::make_shared<QVector<QSharedPointer<GlyphProfile>>>();
};

ProfileManager::~ProfileManager ()
{

}

QSharedPointer<GlyphProfile> ProfileManager::findOrCreate(const ProfileKey &key, bool fixedFont, const QFont &newFont, const QString &fontPath)
{
    return m_profile;
}

QSharedPointer<GlyphProfile> ProfileManager::findOrCreate(const QString &name, int bitmapDimension, bool fixedFont, const QFont &newFont, const QString &fontPath)
{
    ProfileKey key(name, bitmapDimension);
    return findOrCreate(key, fixedFont, newFont, fontPath);
}
