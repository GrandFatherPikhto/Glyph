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

namespace Ui {
class DockGlyph;
}

class DockGlyph : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyph(GlyphManager *glyphManager, QWidget *parent = nullptr);
    ~DockGlyph();

private slots:

    void slotFontChanged(const QFont &font);
    void slotCharacterChanged();
    void slotBitmapDimensionChanged(int newValue);
    void slotGridDimensionChanged(int newValue);
    void slotGlyphSizeChanged(int newValue);
    void slotMoveCenterClicked();
    void slotMoveLeftClicked();
    void slotMoveTopClicked();
    void slotMoveDownClicked();
    void slotMoveRightClicked();

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
    void saveGeometryAndState();
    void restoreGeometryAndState();
    void connectSygnals ();
    void updateGlyph ();

    Ui::DockGlyph *ui;
    GlyphManager *m_glyphManager;
    GlyphPreview *m_glyphPreview;
    QTableView *m_glyphTable;

    QSharedPointer<GlyphMeta> m_glyphMeta;
    int m_gridDimension;
    int m_bitmapDimension;
    int m_glyphSize;
    QFont m_font;
    QString m_fontPath;
    QChar m_character;

    QSplitter *m_mainSplitter;
};

#endif // DOCKGLYPH_H
