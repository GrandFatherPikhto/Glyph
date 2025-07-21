#ifndef DRAWPARAMS_H
#define DRAWPARAMS_H

#include <QObject>
#include <QFont>
#include <QColor>
#include <QSettings>

#include "fontmanager.h"

class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(FontManager *fontManager, QObject *parent = nullptr);

    ~AppSettings();

    void setTemplateColor(const QColor &color) { m_templateColor = color; }
    const QColor & templateColor () {return m_templateColor; }

    void setTemplateBgColor(const QColor &color) { m_templateBgColor = color; }
    const QColor & templateBgColor () {return m_templateBgColor; }

    void setPreviewColor(const QColor &color) { m_previewColor = color; }
    const QColor & previewColor () {return m_previewColor; }

    void setPreviewBgColor(const QColor &color) { m_previewBgColor = color; }
    const QColor & previewBgColor () {return m_previewBgColor; }

    void setDrawColor(const QColor &color) { m_drawColor = color; }
    const QColor & drawColor() { return m_drawColor; }

    void setDrawBgColor(const QColor &color) { m_drawBgColor = color; }
    const QColor & drawBgColor() { return m_drawBgColor; }

    void setGlyphSize(int value) { m_glyphSize = value; }
    int glyphSize() { return m_glyphSize; }

    void setBitmapDimension(int value) { m_bitmapDimension = value; }
    int bitmapDimension () { return m_bitmapDimension; }

    void setFont(const QFont &newFont) { 
        m_font = newFont; 
        if (m_fontManager)
        {
            QString fontPath = m_fontManager->findFontPath(m_font);
        }
    }
    const QFont & font() { return m_font; }

    void setFontPath(const QString &newFontPath) { m_fontPath = newFontPath; }
    const QString & fontPath () { return m_fontPath; }

    void setCharacter(const QChar &newChar)
    {
        m_character = newChar;
    }
    const QChar & character() { return m_character; }

public slots:
    void saveApplicationSettings();
    void restoreApplicationSettings();

signals:

private:
    int m_glyphSize;
    int m_bitmapDimension;

    QChar m_character;

    FontManager *m_fontManager;

    QFont m_font;
    QString m_fontPath;

    QColor m_templateBgColor;
    QColor m_templateColor;
    QColor m_previewBgColor;
    QColor m_previewColor;
    QColor m_drawBgColor;
    QColor m_drawColor;
};

#endif // DRAWPARAMS_H
