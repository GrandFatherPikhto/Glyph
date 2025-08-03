#ifndef PROFILEMANAGER_H_
#define PROFILEMANAGER_H_

#include <QObject>
#include <QString>
#include <QFont>
#include <QSharedPointer>
#include <QHash>

#include "GlyphCore_global.h"

class ProfileKey;
class GlyphProfile;
class AppContext;

class GLYPHCORE_EXPORT ProfileManager : public QObject {
    Q_OBJECT
public:
    ProfileManager(AppContext *appContext);
    ~ProfileManager();

    QSharedPointer<GlyphProfile> findOrCreate(const QString &name, int bitmapDimension, bool fixedFont = true, const QFont &newFont = QFont(), const QString &fontPath = QString());
    QSharedPointer<GlyphProfile> findOrCreate(const ProfileKey &key, bool fixedFont = true, const QFont &newFont = QFont(), const QString &fontPath = QString());

private:
    AppContext *m_appContext;
    std::shared_ptr<QHash<ProfileKey, int>> m_indexes;
    std::shared_ptr<QVector<QSharedPointer<GlyphProfile>>> m_profiles;
    QSharedPointer<GlyphProfile> m_profile;
};

#endif