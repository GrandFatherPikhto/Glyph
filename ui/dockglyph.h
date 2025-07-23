#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QTableView>
#include <QSharedPointer>
#include <QString>
#include <QFont>
#include <QSplitter>

#include "glyphmanager.h"
#include "glyphmeta.h"
#include "glyphpreview.h"
#include "glyphmodel.h"
#include "appcontext.h"

namespace Ui {
class DockGlyph;
}

class DockGlyph : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyph(AppContext *appContext, QWidget *parent = nullptr);
    ~DockGlyph();

private slots:

    void slotFontChanged(const QFont &font);
    void slotCharacterChanged();
    // void slotBitmapDimensionChanged(int newValue);
    // void slotGlyphSizeChanged(int newValue);
    // void slotMoveCenterClicked();
    // void slotMoveLeftClicked();
    // void slotMoveTopClicked();
    // void slotMoveDownClicked();
    // void slotMoveRightClicked();

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

    Ui::DockGlyph *ui;
    AppContext *m_appContext;
    GlyphPreview *m_glyphPreview;
    QTableView *m_glyphTable;

    // QSharedPointer<GlyphMeta> m_glyphMeta;
    
    QSplitter *m_mainSplitter;

    GlyphModel *m_glyphModel;
};

#endif // DOCKGLYPH_H
