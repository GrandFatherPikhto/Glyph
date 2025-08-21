#ifndef MAINSTATUSBAR_H
#define MAINSTATUSBAR_H

#include <QObject>
#include <QToolBar>
#include <QWidget>
#include <QAction>

class AppContext;
class AppSettings;

class MainToolbar : public QToolBar
{
    Q_OBJECT
public:
    MainToolbar(AppContext *appContext, QWidget *parent = nullptr);
    ~MainToolbar();

signals:
    void gridEnable(bool enable);
    void templateLayerEnable(bool editable);
    void previewLayerEnable(bool enable);
    void userLayerEnable(bool enable);
    void glyphRectLayerEnable(bool enable);
    void baselineLayerEnable(bool enable);
    void leftlineLayerEnable(bool enable);
    void bitmapRectLayerEnable(bool enable);
    void pasteGlyphToDrawLayer();
    void clearDrawLayer();

private:
    void setupUi();
    void setupSignals ();
    void setupStates ();

    AppContext *m_appContext;
    AppSettings *m_appSettings;

    QAction *m_gridEnable;
    QAction *m_templateLayerEnable;
    QAction *m_previewLayerEnable;
    QAction *m_drawLayerEnable;
    QAction *m_glyphRectLayerEnable;
    QAction *m_bitmapRectLayerEnable;
    QAction *m_leftLineLayerEnable;
    QAction *m_baselineLayerEnable;
    QAction *m_pasteGlyphToDrawLayer;
    QAction *m_clearDrawLayer;
};

#endif // MAINSTATUSBAR_H
