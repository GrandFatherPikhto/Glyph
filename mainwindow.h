#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QGridLayout>
#include <QTabWidget>
#include <QDebug>
#include <QLabel>

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
    void templateGlyphEnabled(bool editable);
    void contourEnabled(bool enable);
    void gridEnabled(bool enable);
    void glyphGridEnabled(bool enable);

private slots:
    void on_action_Quit_triggered();

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
    void setStatusBarGridSize (int newGridSize);
    void saveGeometryAndState();
    void restoreGeometryAndState();

    Ui::MainWindow *ui;

    DockGlyph *m_dockGlyph;
    GlyphWidget *m_glyphWidget;
    QGridLayout *m_mainLayout;
    QToolBar *m_glyphToolBar;

    QAction *m_templateGlyphEnable;
    QAction *m_gridEnable;
    QAction *m_contourEnable;
    QAction *m_glyphGrid;


    QLabel *m_fontLabel;
    QLabel *m_glyphSizeLabel;
    QLabel *m_gridSizeLabel;
    QLabel *m_charLabel;

    GlyphManager *m_glyphManager;

};
#endif // MAINWINDOW_H
