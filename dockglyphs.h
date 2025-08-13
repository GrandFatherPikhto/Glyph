#ifndef DOCKGLYPHS_H
#define DOCKGLYPHS_H

#include <QDockWidget>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSettings>

#include "profilecontext.h"

namespace Ui {
class DockGlyphs;
}

class AppContext;
class AppSettings;
class ProfileManager;
class CharmapManager;
class GlyphManager;
class GlyphModel;

class DockGlyphs : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphs(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphs();

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void setupValues();
    void setupSignals();
    void refreshGlyphsTable ();

    void saveDockGlyphsSettings();
    void restoreDockGlyphsSettings();

    Ui::DockGlyphs *ui;

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    CharmapManager *m_charmapManager;
    GlyphManager *m_glyphManager;

    GlyphModel *m_glyphsModel;

    ProfileContext m_profile;
};

#endif // DOCKGLYPHS_H
