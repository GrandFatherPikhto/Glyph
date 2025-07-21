#ifndef MAINSTATUSBAR_H
#define MAINSTATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QWidget>
#include <QLabel>
#include <QSettings>
#include <QSharedPointer>
#include <QString>

#include "appcontext.h"
#include "glyphmeta.h"

class MainStatusbar : public QStatusBar
{
    Q_OBJECT
public:
    MainStatusbar(AppContext *appContext, QWidget* parent = nullptr);
    ~MainStatusbar();

public slots:
    void setGlyph(QSharedPointer<GlyphMeta> glyphMeta);

private:
    void setupUI();
    void saveToolbarState();
    void restoreToolbarState();

    AppContext *m_appContext;
    
    QSharedPointer<GlyphMeta> m_glyphMeta;

    QLabel *m_fontLabel;
    QLabel *m_glyphSizeLabel;
    QLabel *m_gridSizeLabel;
    QLabel *m_glyphRectLabel;
    QLabel *m_charLabel;
};

#endif // MAINSTATUSBAR_H
