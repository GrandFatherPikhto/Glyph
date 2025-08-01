#ifndef DOCKGLYPHSELECTOR_H
#define DOCKGLYPHSELECTOR_H

#include <QDockWidget>
#include <QSettings>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>
#include <QItemSelection>

#include "glyphui_global.h"

class AppContext;
class AppSettings;
class FontManager;
class GlyphManager;
class FontCharacterModel;
class FontScriptModel;
class FontCategoryModel;
class FontDecompositionModel;

namespace Ui {
class DockGlyphSelector;
}

class GLYPHUI_EXPORT DockGlyphSelector : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphSelector(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphSelector();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void closeEvent (QCloseEvent *event) override;

private:
    void setupValues ();
    void setupModels ();
    void setupSignals ();

    void setFromToFilter ();
    void clickCharacter(const QModelIndex &index);
    void doubleClickCharacter(const QModelIndex &index);

    void setFontCategoryFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontScriptFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontDecompositionFilter(const QItemSelection &selected, const QItemSelection &deselected);

    void saveDockGlyphSelectorState ();
    void restoreDockGlyphSelectorState ();

    Ui::DockGlyphSelector *ui;

    AppContext *m_appContext;
    
    AppSettings * m_appSettings;
    FontManager * m_fontManager;
    GlyphManager * m_glyphManager;

    FontCharacterModel *m_fontCharacterModel;
    FontScriptModel *m_scriptModel;
    FontCategoryModel *m_categoryModel;
    FontDecompositionModel *m_decompositionModel;
};

#endif // DOCKGLYPHSELECTOR_H
