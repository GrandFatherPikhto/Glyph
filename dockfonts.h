#ifndef DOCKFONTS_H
#define DOCKFONTS_H

#include <QDockWidget>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>
#include <QSqlQueryModel>

class AppContext;
class FontModel;
class FontManager;

namespace Ui {
class DockFonts;
}

class DockFonts : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockFonts(AppContext *appContext, QWidget *parent = nullptr);
    ~DockFonts();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void setupValues();
    void setupSignals();
    void refreshFontsTable();
    int  getFontId(int row);
    void saveDockFontsSettings();
    void restoreDockFontsSettings();

    Ui::DockFonts *ui;
    AppContext *m_appContext;
    FontManager *m_fontManager;
    QSqlQueryModel *m_fontModel;
};

#endif // DOCKFONTS_H
