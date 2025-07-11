#ifndef DOCKGLYPH_H
#define DOCKGLYPH_H

#include <QDockWidget>
#include <QString>
#include <QFont>

namespace Ui {
class DockGlyph;
}

class DockGlyph : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGlyph(QWidget *parent = nullptr);
    ~DockGlyph();

private slots:

    // void on_gridSize_valueChanged(int arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_character_returnPressed();

    void on_character_editingFinished();

    void on_gridSize_valueChanged(int arg1);

    void on_glyphSize_valueChanged(int arg1);

signals:
    void gridSizeChanged(int newSize);
    void glyphSizeChanged(int newSize);
    void fontChanged(const QFont &newFont, const QString &newFontPath);
    void characterChanged(const QChar &newChar);
    void glyphParamsChanged(const QFont &newFont, const QString &newFontPath, int newFontSize, const QChar &newCharacter, int newGridSize);

private:
    Ui::DockGlyph *ui;

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
