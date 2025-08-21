#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>

class AppContext;
class DockCharmap;
class DockProfiles;
class DockGlyphs;
class DockFonts;
class MainToolbar;

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

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public slots:
    void quitApplication();
    void createProject();
    void setupValues();

private:
    void setupSignals();
    void saveGeometryAndState();
    void restoreGeometryAndState();

    Ui::MainWindow *ui;

    AppContext *m_appContext;
    DockCharmap *m_dockCharmap;
    DockProfiles *m_dockProfiles;
    DockGlyphs *m_dockGlyphs;
    DockFonts *m_dockFonts;
    MainToolbar *m_mainToolbar;
};
#endif // MAINWINDOW_H
