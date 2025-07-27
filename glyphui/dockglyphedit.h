#ifndef DOCKGLYPHEDIT_H
#define DOCKGLYPHEDIT_H

#include <QDockWidget>
#include <QVBoxLayout>

#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>

#include "appcontext.h"
#include "glyphui_global.h"

#include "glyphpreview.h"
#include "glyphmodel.h"
#include "bitmapdimensionmodel.h"

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
    void setupDockGlyphEditModels ();
    void setupDockGlyphEditUI();
    void setupSignals ();

    void saveGeometryAndState();
    void restoreGeometryAndState ();

    Ui::DockGlyphEdit *ui;
    AppContext *m_appContext;
    GlyphPreview *m_glyphPreview;
    GlyphModel *m_glyphModel;
    BitmapDimensionModel *m_bitmapDimensionModel;
};

#endif // DOCKGLYPHEDIT_H
