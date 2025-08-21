#ifndef DOCKPROFILES_H
#define DOCKPROFILES_H

#include <QDockWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "gridcontext.h"
#include "profilecontext.h"

class AppContext;
class AppSettings;
class ProfileManager;
class GlyphManager;
class FontManager;
class GridManager;

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
    void loadProfileContext ();
    void loadGridContext ();
    GridContext readGridContext();
    ProfileContext readProfileContext ();
    
    Ui::DockProfiles *ui;

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    GridManager *m_gridManager;

    QSqlQueryModel *m_profilesModel;
};

#endif // DOCKPROFILES_H
