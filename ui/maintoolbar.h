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

#include "glyphmanager.h"

class MainToolbar : public QToolBar
{
    Q_OBJECT
public:
    MainToolbar (GlyphManager *glyphManager, QWidget *parent = nullptr);
    ~MainToolbar ();

signals:    
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

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI();
    void saveToolbarState();
    void restoreToolbarState();

    GlyphManager *m_glyphManager;

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
};

#endif // MAINTOOLBAR_H
