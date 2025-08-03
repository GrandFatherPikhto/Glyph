#ifndef DOCKGLYPHSETPROFILE_H
#define DOCKGLYPHSETPROFILE_H

#include <QDockWidget>

namespace Ui {
class DockGlyphProfiles;
}

class AppContext;
class AppSettings;

class DockGlyphProfiles : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphProfiles(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphProfiles();

private:
    Ui::DockGlyphProfiles *ui;

    void initValues ();

    AppContext *m_appContext;
    AppSettings *m_appSettings;
};

#endif // DOCKGLYPHSETPROFILE_H
