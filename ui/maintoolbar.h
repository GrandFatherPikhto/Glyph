#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QWidget>

#include <QAction>
#include <QSpinBox>
#include <QSettings>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>
#include <QLabel>

#include "appcontext.h"

class MainToolbar : public QToolBar
{
    Q_OBJECT
public:
    MainToolbar (AppContext *appContext, QWidget *parent = nullptr);
    ~MainToolbar ();

signals:    
    void gridEnable(bool enable);
    void templateLayerEnable(bool editable);
    void previewLayerEnable(bool enable);
    void userLayerEnable(bool enable);
    void glyphRectLayerEnable(bool enable);
    void baselineLayerEnable(bool enable);
    void bitmapRectLayerEnable(bool enable);
    void leftGridCellsChanged(int value);
    void bottomGridCellsChanged(int value);
    void pasteGlyphToUserLayer();
    void clearUserLayer();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI();
    void setupSignals();
    void initState ();
    void saveToolbarState();
    void restoreToolbarState();

    AppContext *m_appContext;

    QAction *m_gridEnable;
    QAction *m_templateLayerEnable;
    QAction *m_previewLayerEnable;
    QAction *m_userLayerEnable;
    QAction *m_glyphRectLayerEnable;
    QAction *m_bitmapRectLayerEnable;
    QAction *m_baselineLayerEnable;
    QAction *m_pasteGlyphToUserLayer;
    QAction *m_clearUserLayer;

    QLabel *m_leftCellsLabel;
    QLabel *m_bottomCellsLabel;

    QSpinBox *m_addLeftCells;
    QSpinBox *m_addBottomCells;
};

#endif // MAINTOOLBAR_H
