#ifndef DOCKGLYPHEDIT_H
#define DOCKGLYPHEDIT_H

#include <QDockWidget>
#include <QVBoxLayout>

#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>

#include <QModelIndex>

#include "glyphui_global.h"

class AppContext;
class AppSettings;
class GlyphManager;
class GlyphPreview;
class GlyphModel;
class GlyphFilter;
class BitmapDimensionModel;
class DimensionManager;

namespace Ui {
class DockGlyphEdit;
}

class GLYPHUI_EXPORT DockGlyphEdit : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphEdit(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphEdit();

protected:
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;    

private:
    void setupVariables ();
    void restoreDefaultValues ();
    void setupDockGlyphEditModels ();
    void setupDockGlyphEditUI();
    void setupSignals ();

    void saveGeometryAndState();
    void restoreGeometryAndState ();
    void syncOutDimensionManager (int value = -1);
    void syncInDimensionManager  (const QModelIndex &current, const QModelIndex &previous);

    void doubleClickGlyph (const QModelIndex &index);
    void clickGlyph (const QModelIndex &index);

    Ui::DockGlyphEdit *ui;
    BitmapDimensionModel *m_bitmapDimensionModel;
    GlyphPreview *m_glyphPreview;
    GlyphModel *m_glyphModel;

    AppContext *m_appContext;

    AppSettings * m_appSettings;
    GlyphManager * m_glyphManager;
    DimensionManager * m_dimensionManager;
    GlyphFilter * m_glyphFilter;
};

#endif // DOCKGLYPHEDIT_H
