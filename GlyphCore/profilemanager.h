#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QString>

#include "GlyphCore_global.h"

#include "profilecontext.h"

class AppContext;
class GlyphContext;
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

    GlyphContext defaultGlyphContext(const QChar &ch, bool temporary = true);
    bool defaultGlyphContext(GlyphContext &context);

signals:
    void profilesChanged();
    void changeProfile(const ProfileContext &profile);
    void profileChanged(const ProfileContext &profile);

private:
    void setupValues ();
    void setupSignals ();
    bool createTable();

    void saveSettings();
    void restoreSettings();

    AppContext *m_appContext;
    FontManager *m_fontManager;

    QString m_tableName;

    ProfileContext m_profile;
};

#endif // PROFILEMANAGER_H
