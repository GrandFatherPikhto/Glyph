#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <QObject>
#include <QFont>
#include <QString>
#include <QSharedPointer>

#include "GlyphCore_global.h"

#include "glyphprofile.h"

class AppContext;
class FontManager;
class ProfileManager;

class GLYPHCORE_EXPORT AppSettings : public QObject {
    Q_OBJECT
public:
    explicit AppSettings(AppContext *appContext);
    ~AppSettings ();

    const GlyphProfile & glyphProfile() const;
    void setGlyphProfile(const GlyphProfile &profile);
    const QString & appDataPath () const;
    GlyphProfile defaultGlyphProfile();

signals:
    void changeGlyphProfile(const GlyphProfile &profile);
    void glyphProfileChanged (const GlyphProfile &profile);

private:
    void setupValues();
    void setupSignals();
    void saveAppSettings();
    void restoreAppSettings();
    const QString & initAppDataCatalog();


    AppContext *m_appContext;
    FontManager *m_fontManager;
    ProfileManager *m_profileManager;

    GlyphProfile m_glyphProfile;
    QString m_appDataPath;
};

#endif // APPSETTINGS_H_
