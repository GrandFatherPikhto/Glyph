#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QTableView>
#include <QSharedPointer>
#include <QString>
#include <QFont>
#include <QSplitter>

#include "glyphui_global.h"

class GlyphModel;
class GlyphManager;
class GlyphContext;
class GlyphPreview;
class AppContext;
class AppSettings;

namespace Ui {
class DockGlyphTable;
}

class AppContext;
class AppSettings;
class GlyphManager;
class GlyphPreview;
class GlyphModel;

class GLYPHUI_EXPORT DockGlyphTable : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphTable(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphTable();

private slots:

signals:
    void glyphChanged(QSharedPointer<GlyphContext> glyphContext);
    void bitmapDimensionChanged(int newSize);
    void gridDimensionChanged(int newValue);
    void glyphSizeChanged(int newSize);
    void fontChanged(const QFont &newFont, const QString &newFontPath);
    void characterChanged(const QChar &newChar);
    void moveGlyphLeft ();
    void moveGlyphRight();
    void moveGlyphTop ();
    void moveGlyphDown ();
    void moveGlyphCenter ();

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void setupValues ();
    void restoreData ();
    void saveGeometryAndState();
    void restoreGeometryAndState();
    void connectSygnals ();
    void updateGlyph ();

    Ui::DockGlyphTable *ui;

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    GlyphManager *m_glyphManager;
    GlyphPreview *m_glyphPreview;
    GlyphModel *m_glyphModel;
    
};

#endif // DOCKGLYPH_H
