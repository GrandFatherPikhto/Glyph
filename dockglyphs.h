#ifndef DOCKGLYPHS_H
#define DOCKGLYPHS_H

#include <QDockWidget>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include "glyphprofile.h"

namespace Ui {
class DockGlyphs;
}

class AppContext;
class AppSettings;
class ProfileManager;
class CharmapManager;

class DockGlyphs : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphs(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphs();

private:
    void setupValues();
    void setupSignals();
    void refreshProfilesComboBox ();
    void setCurrentProfile(int idx);

    Ui::DockGlyphs *ui;

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    CharmapManager *m_charmapManager;

    QSqlQueryModel *m_profilesModel;

    GlyphProfile m_glyphProfile;
};

#endif // DOCKGLYPHS_H
