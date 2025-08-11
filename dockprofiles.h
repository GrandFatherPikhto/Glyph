#ifndef DOCKPROFILES_H
#define DOCKPROFILES_H

#include <QDockWidget>
#include <QSqlTableModel>

#include "glyphprofile.h"

class AppContext;
class AppSettings;
class ProfileManager;

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
    void updateProfilesTable();
    void setupValues ();
    void setupSignals ();
    void loadGlyphProfile ();
    
    Ui::DockProfiles *ui;
    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;

    QSqlTableModel *m_profilesModel;

    GlyphProfile m_glyphProfile;
};

#endif // DOCKPROFILES_H
