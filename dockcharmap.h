#ifndef DOCKCHARMAP_H
#define DOCKCHARMAP_H

#include <QDockWidget>
#include <QApplication>
#include <QTableView>
#include <QListView>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSharedPointer>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QSettings>
#include <QCloseEvent>
#include <QShowEvent>
#include <QHideEvent>

#include "profilecontext.h"

class AppContext;
class CharmapManager;
class AppSettings;
class CharmapModel;
class UnicodeMetadataSelectionModel;
class SqlFilter;
class GlyphManager;
class ProfileManager;

namespace Ui {
class DockCharmap;
}

class DockCharmap : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockCharmap(AppContext *appContext, QWidget *parent = nullptr);
    ~DockCharmap();

    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void setupSignals ();
    void setupValues  ();

    void initFontComboBox ();

    void initCharmapTable ();

    void initCategoriesList ();
    void initScriptsList ();
    void initDecompositionsList ();

    void refreshCharmapTable ();
    void refreshCategoriesList ();
    void refreshScriptsList ();
    void refreshDecompositionsList ();

    void glyphClicked(const QModelIndex &index);
    void glyphDoubleClicked(const QModelIndex &index);
    void setResetGlyph(int row);

    void saveDockCharmapState ();
    void restoreDockCharmapState ();

    AppContext *m_appContext;
    CharmapManager *m_charmapManager;
    AppSettings *m_appSettings;
    GlyphManager *m_glyphManager;
    ProfileManager *m_profileManager;

    SqlFilter *m_filter;

    ProfileContext m_profile;

    Ui::DockCharmap *ui;

    CharmapModel *m_charmapModel;

    QFont m_font;

    // CharmapQuery *m_charmapQuery;

    int m_unicodeMSB;

    QSqlQueryModel *m_categoryModel;
    QSqlQueryModel *m_scriptModel;
    QSqlQueryModel *m_decompostionModel;

    UnicodeMetadataSelectionModel *m_categorySelectionModel;
    UnicodeMetadataSelectionModel *m_scriptSelectionModel;
    UnicodeMetadataSelectionModel *m_decompositionSelectionModel;

    // QSharedPointer<Condition> m_categoryCondition;
    // QSharedPointer<Condition> m_scriptCondition;
    // QSharedPointer<Condition> m_decompositionCondition;
};


#endif // DOCKCHARMAP_H
