#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QSharedPointer>
#include <QString>
#include <QFont>

#include "glyphmanager.h"
#include "glyphmeta.h"

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
    void slotGridSizeChanged(int gridSize);
    void slotGlyphSizeChanged(int glyphSize);
    void slotMoveCenterClicked();
    void slotMoveLeftClicked();
    void slotMoveTopClicked();
    void slotMoveDownClicked();
    void slotMoveRightClicked();

signals:
    void glyphChanged(QSharedPointer<GlyphMeta> glyph);
    void gridSizeChanged(int newSize);
    void glyphSizeChanged(int newSize);
    void fontChanged(const QFont &newFont, const QString &newFontPath);
    void characterChanged(const QChar &newChar);
    void moveGlyphLeft ();
    void moveGlyphRight();
    void moveGlyphTop ();
    void moveGlyphDown ();
    void moveGlyphCenter ();

private:
    void connectSygnals ();
    void updateGlyph ();

    Ui::DockGlyph *ui;
    GlyphManager *m_glyphManager;

    QSharedPointer<GlyphMeta> m_glyphMeta;
    int m_gridSize;
    int m_glyphSize;
    QFont m_font;
    QString m_fontPath;
    QChar m_character;


    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // DOCKGLYPH_H
