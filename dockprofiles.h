#ifndef DOCKPROFILES_H
#define DOCKPROFILES_H

#include <QDockWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "profilecontext.h"
#include "fontcontext.h"
#include "profilecontext.h"

class AppContext;
class AppSettings;
class ProfileManager;
class FontManager;

namespace Ui {
class DockProfiles;
}

class DockProfiles : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockProfiles(AppContext *appContext, QWidget *parent = nullptr);
    ~DockProfiles();

private:
    void updateProfilesCombo();
    void setupValues ();
    void setupSignals ();
    void loadProfileContext ();
    
    Ui::DockProfiles *ui;
    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    FontManager *m_fontManager;
    QSqlQueryModel *m_profilesModel;

    ProfileContext m_profile;
    FontContext m_font;
};

#endif // DOCKPROFILES_H
