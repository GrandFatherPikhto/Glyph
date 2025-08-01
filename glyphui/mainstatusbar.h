#ifndef MAINSTATUSBAR_H
#define MAINSTATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QWidget>
#include <QLabel>
#include <QSettings>
#include <QSharedPointer>
#include <QString>

#include "glyphui_global.h"

class GlyphContext;
class AppContext;
class GlyphManager;

class GLYPHUI_EXPORT MainStatusbar : public QStatusBar
{
    Q_OBJECT
public:
    MainStatusbar(AppContext *appContext, QWidget* parent = nullptr);
    ~MainStatusbar();

public slots:
    void setGlyph(QSharedPointer<GlyphContext> glyphContext);

private:
    void initValues ();
    void setupUI();
    void setupSignals ();
    void saveToolbarState();
    void restoreToolbarState();

    AppContext *m_appContext;
    
    GlyphManager * m_glyphManager;
    
    QLabel *m_fontLabel;
    QLabel *m_glyphSizeLabel;
    QLabel *m_gridSizeLabel;
    QLabel *m_glyphRectLabel;
    QLabel *m_charLabel;
};

#endif // MAINSTATUSBAR_H
