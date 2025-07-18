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
#include "glyphmanager.h"
#include "glyphwidget.h"

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
    void setupSignals ();
    void setupGlyphWidget ();
    void setupGlyphDock ();
    void setupStatusBar();
    void setupGlyphToolBar();
    void setStatusBarFontName(const QFont &newFont);
    void setStatusBarCharacter(const QChar &newCharacter);
    void setStatusBarGlyphSize(int newGlyphSize);
    void setStatusBarBitmapDimension (int newGridSize);
    void saveGeometryAndState();
    void restoreGeometryAndState();

    Ui::MainWindow *ui;

    DockGlyph *m_dockGlyph;
    GlyphWidget *m_glyphWidget;
    QGridLayout *m_mainLayout;
    QToolBar *m_glyphToolBar;

    QAction *m_gridEnable;
    QAction *m_templateLayerEnable;
    QAction *m_previewLayerEnable;
    QAction *m_userLayerEnable;
    QAction *m_glyphRectLayerEnable;
    QAction *m_bitmapRectLayerEnable;
    QAction *m_baselineLayerEnable;
    QAction *m_pasteGlyphToUserLayer;
    QAction *m_clearUserLayer;

    QSpinBox *m_addLeftCells;
    QSpinBox *m_addBottomCells;


    QLabel *m_fontLabel;
    QLabel *m_glyphSizeLabel;
    QLabel *m_gridSizeLabel;
    QLabel *m_glyphRectLabel;
    QLabel *m_charLabel;

    GlyphManager *m_glyphManager;

};
#endif // MAINWINDOW_H
