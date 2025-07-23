#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QGridLayout>
#include <QTabWidget>
#include <QDebug>
#include <QLabel>
#include <QSpinBox>

#include "dockglyph.h"
#include "dockglyphselector.h"
#include "appcontext.h"
#include "glyphwidget.h"
#include "maintoolbar.h"
#include "mainstatusbar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void saveCurrentState ();
    void gridEnable(bool enable);
    void templateLayerEnable(bool editable);
    void previewLayerEnable(bool enable);
    void userLayerEnable(bool enable);
    void glyphRectLayerEnable(bool enable);
    void baselineLayerEnable(bool enable);
    void bitmapRectLayerEnable(bool enable);
    void leftGridCells(int value);
    void bottomGridCells(int value);
    void pasteGlyphToUserLayer();
    void clearUserLayer();

private slots:
    void slotActionQuitTriggered();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    void setupMainToolbar ();
    void setupSignals ();
    void setupGlyphWidget ();
    void setupDockPanels ();
    void setupStatusBar();
    void saveGeometryAndState();
    void restoreGeometryAndState();

    Ui::MainWindow *ui;

    DockGlyph *m_dockGlyph;
    MainToolbar *m_mainToolbar;
    MainStatusbar *m_mainStatubar;

    DockGlyphSelector *m_dockGlyphSelector;

    GlyphWidget *m_glyphWidget;
    QGridLayout *m_centralLayout;

    AppContext *m_appContext;
};
#endif // MAINWINDOW_H
