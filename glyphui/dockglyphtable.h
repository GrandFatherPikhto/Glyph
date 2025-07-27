#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QTableView>
#include <QSharedPointer>
#include <QString>
#include <QFont>
#include <QSplitter>

#include "glyphui_global.h"

// #include "glyphmanager.h"
#include "glyphmeta.h"
#include "glyphpreview.h"
#include "glyphmodel.h"
#include "appcontext.h"

namespace Ui {
class DockGlyphTable;
}

class GLYPHUI_EXPORT DockGlyphTable : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyphTable(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyphTable();

private slots:

signals:
    void glyphChanged(QSharedPointer<GlyphMeta> glyph);
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
    void restoreData();
    void saveGeometryAndState();
    void restoreGeometryAndState();
    void connectSygnals ();
    void updateGlyph ();

    Ui::DockGlyphTable *ui;
    AppContext *m_appContext;
    GlyphPreview *m_glyphPreview;
    // QTableView *m_glyphTable;

    // QSharedPointer<GlyphMeta> m_glyphMeta;
    
    // QSplitter *m_mainSplitter;

    GlyphModel *m_glyphModel;
};

#endif // DOCKGLYPH_H
