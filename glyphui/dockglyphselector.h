#ifndef DOCKGLYPHSELECTOR_H
#define DOCKGLYPHSELECTOR_H

#include <QDockWidget>
#include <QSettings>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>

#include "glyphui_global.h"

#include "appcontext.h"
#include "fontcharactermodel.h"
#include "fontcategorymodel.h"
#include "fontdecompositionmodel.h"
#include "fontscriptmodel.h"

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
    void selectCharacter(const QModelIndex &index);
    void setCharacter(const QModelIndex &index);

    void setFontCategoryFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontScriptFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontDecompositionFilter(const QItemSelection &selected, const QItemSelection &deselected);

    void saveDockGlyphSelectorState ();
    void restoreDockGlyphSelectorState ();

    Ui::DockGlyphSelector *ui;
    AppContext *m_appContext;

    FontCharacterModel *m_fontCharacterModel;
    FontScriptModel *m_scriptModel;
    FontCategoryModel *m_categoryModel;
    FontDecompositionModel *m_decompositionModel;
};

#endif // DOCKGLYPHSELECTOR_H
