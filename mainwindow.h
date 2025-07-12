#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDebug>
#include <QLabel>

#include "dockglyph.h"

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
    
private slots:
    void on_action_Quit_triggered();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    void connectGlyphDockEvents();
    void setupStatusBar();
    void setStatusBarFontName(const QFont &newFont);
    void setStatusBarCharacter(const QChar &newCharacter);
    void setStatusBarGlyphSize(int newGlyphSize);
    void setStatusBarGridSize (int newGridSize);
    void saveGeometryAndState();
    void restoreGeometryAndState();

    Ui::MainWindow *ui;

    DockGlyph *m_dockGlyph;

    QLabel *m_fontLabel;
    QLabel *m_glyphSizeLabel;
    QLabel *m_gridSizeLabel;
    QLabel *m_charLabel;
};
#endif // MAINWINDOW_H
