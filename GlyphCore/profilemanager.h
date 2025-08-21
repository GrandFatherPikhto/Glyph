#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

#include "GlyphCore_global.h"

#include "profilecontext.h"

class AppContext;
class GlyphContext;
class AppSettings;
class FontManager;

class GLYPHCORE_EXPORT ProfileManager : public QObject
{
    Q_OBJECT
public:
    ProfileManager(AppContext *appContext);
    ~ProfileManager();

    bool insertOrReplaceProfile(const ProfileContext &profile);
    bool clearTable();
    const QString & tableName() const;

    const ProfileContext & profile() const { return m_profile; }

    bool getProfileById(int id, ProfileContext &profile);
    bool findProfile(ProfileContext &profile);

    ProfileContext glyphProfile(const GlyphContext &glyph);
    // GlyphContext defaultGlyphContext(const QChar &ch);
    // bool defaultGlyphContext(GlyphContext &context);
    void defaultProfile(ProfileContext &context);
    bool assignQueryWithProfile(QSqlQuery query, ProfileContext &profile);

signals:
    void profilesChanged();
    void changeProfile(const ProfileContext &profile);
    void profileChanged(const ProfileContext &profile);
    void profileAppended(ProfileContext profile);

private:
    void setupValues ();
    void setupSignals ();
    bool createTable();

    void saveSettings();
    void restoreSettings();

    AppContext *m_appContext;
    FontManager *m_fontManager;
    AppSettings *m_appSettings;

    QString m_tableName;

    ProfileContext m_profile;
};

#endif // PROFILEMANAGER_H
