#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QSharedPointer>
#include <QString>
#include <QFont>

#include "glyphmanager.h"
#include "glyph.h"

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

    // void on_gridSize_valueChanged(int arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_character_returnPressed();

    void on_character_editingFinished();

    void on_gridSize_valueChanged(int arg1);

    void on_glyphSize_valueChanged(int arg1);

    void on_moveCenter_clicked();

    void on_moveLeft_clicked();

    void on_moveTop_clicked();

    void on_moveDown_clicked();

    void on_moveRight_clicked();

signals:
    void glyphChanged(QSharedPointer<Glyph> glyph);
    void gridSizeChanged(int newSize);
    void glyphSizeChanged(int newSize);
    void fontChanged(const QFont &newFont, const QString &newFontPath);
    void characterChanged(const QChar &newChar);
    void glyphParamsChanged(const QFont &newFont, const QString &newFontPath, int newFontSize, const QChar &newCharacter, int newGridSize);
    void moveGlyphLeft ();
    void moveGlyphRight();
    void moveGlyphTop ();
    void moveGlyphDown ();
    void moveGlyphCenter ();

private:
    void updateGlyph ();

    Ui::DockGlyph *ui;
    GlyphManager *m_glyphManager;

    QSharedPointer<Glyph> m_glyph;
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
