#ifndef COLORS_H
#define COLORS_H

#include <QChar>
#include <QObject>
#include <QColor>
#include <QSettings>
#include <QFont>
#include <QMargins>

#include "gridpaddings.h"

class AppSettings : public QObject
{
    Q_OBJECT

public:
    AppSettings(QObject *parent = nullptr);
    ~AppSettings();

    void setTemplateColor(const QColor &color) { 
        if (m_templateColor != color)
        {
            m_templateColor = color;
            emit templateColorChanged(m_templateColor);
        }
    }

    void setTemplateBgColor(const QColor &color) {
        if( m_templateBgColor != color)
        {
            m_templateBgColor = color;
            emit templateBgColorChanged(m_templateBgColor);
        }
    }

    void setPreviewColor(const QColor &color) { 
        if (m_previewColor != color)
        {
            m_previewColor = color;
            emit previewColorChanged(m_previewColor);
        }
    }

    void setPreviewBgColor(const QColor &color) {
        if (m_previewBgColor != color)
        {
            m_previewBgColor = color;
            emit previewBgColorChanged(m_previewBgColor);
        }
    }

    void setDrawColor(const QColor &color) { 
        if (m_drawColor != color)
        {
            m_drawColor = color;
            emit drawColorChanged(m_drawColor);
        }
    }

    void setDrawBgColor(const QColor &color) {
        if (m_drawBgColor != color)
        {
            m_drawBgColor = color;
            emit drawBgColorChanged(m_drawBgColor);
        }
    }

    void setGlyphSize (int value)
    {
        if (m_glyphSize != value)
        {
            m_glyphSize = value;
            emit glyphSizeChanged(m_glyphSize);
        }
    }

    void setBitmapDimension (int value)
    {
        if (m_bitmapDimension != value)
        {
            m_bitmapDimension = value;
            emit bitmapDimensionChanged(m_bitmapDimension);
        }
    }

    void setGlyphFont(const QFont &font)
    {
        if (m_font.family() != font.family())
        {
            m_font = font;
            emit glyphFontChanged(m_font);
        }
    }

    void setGlyphCharacter(const QChar &value)
    {
        if (m_character != value)
        {
            m_character = value;
            emit glyphCharacterChanged(m_character);
        }
    }

    void setGridPaddings(const GridPaddings &value)
    {
        if (m_gridPaddings != value)
        {
            m_gridPaddings = value;
            emit gridPaddingsChanged(m_gridPaddings);
        }
    }

    void setGlyphPreviewMargins (const QMargins &value)
    {
        if (m_glyphPreviewMargins != value)
        {
            m_glyphPreviewMargins = value;
            emit glyphPreviewMarginsChanged(m_glyphPreviewMargins);
        }
    }

    void setGlyphWidgetMargins (const QMargins &value)
    {
        if (m_glyphWidgetMargins != value)
        {
            m_glyphWidgetMargins = value;
            emit glyphWidgetMarginsChanged(m_glyphWidgetMargins);
        }
    }

        void setGridLayerEnable (bool value) { 
        if (m_gridLayerEnable != value)
        {
            m_gridLayerEnable = value; 
            emit gridLayerEnableChanged(m_gridLayerEnable);
        }
    }

    void setTemplateLayerEnable (bool value) {
        if (m_templateLayerEnable != value)
        {
            m_templateLayerEnable = value;
            emit templateLayerEnableChanged(m_templateLayerEnable);
        } 
    }

    void setPreviewLayerEnable (bool value) { 
        if (m_previewLayerEnable != value)
        {
            m_previewLayerEnable = value;
            emit previewLayerEnableChanged(m_previewLayerEnable);
        } 
    }

    void setDrawLayerEnable (bool value) {
        if(m_drawLayerEnable != value)
        {
            m_drawLayerEnable = value;
            emit drawLayerEnableChanged(m_drawLayerEnable);
        }
    }

    void setGlyphRectLayerEnable (bool value) { 
        if (m_glyphRectLayerEnable != value)
        {
            m_glyphRectLayerEnable = value;
            emit glyphRectLayerEnableChanged (m_glyphRectLayerEnable);
        }
    }

    void setBaselineLayerEnable (bool value) { 
        if (m_baselineLayerEnable != value)
        {
            m_baselineLayerEnable = value;
            emit baselineLayerEnableChanged(m_baselineLayerEnable);
        }
    }

    void setFontMSB(int value)
    {
        if (m_fontMSB != value)
        {
            m_fontMSB = value;
            emit fontMSBChanged(m_fontMSB);
        }
    }

    void setBitmapRectLayerEnable (bool value) {
        if (m_bitmapRectLayerEnable != value)
        {
            m_bitmapRectLayerEnable = value;
            emit bitmapRectLayerEnableChanged(m_bitmapRectLayerEnable);
        }
    }

    const QColor & templateColor() { return m_templateColor; }
    const QColor & templateBgColor() { return m_templateBgColor; }
    const QColor & previewColor () { return m_previewColor; }
    const QColor & previewBgColor () { return m_previewBgColor; }
    const QColor & drawColor () { return m_drawColor; }
    const QColor & drawBgColor () { return m_drawBgColor; }

    int glyphSize() { return m_glyphSize; }
    int bitmapDimension() { return m_bitmapDimension; }
    const QFont & glyphFont() { return m_font; }
    const QChar & glyphCharacter() { return m_character; }

    const GridPaddings & gridPaddings () { return m_gridPaddings; }
    const QMargins & glyphPreviewMargins () { return m_glyphPreviewMargins; }
    const QMargins & glyphWidgetMargins () { return m_glyphWidgetMargins; }

    bool gridLayerEnable () { return m_gridLayerEnable; }
    bool templateLayerEnable () { return m_templateLayerEnable; }
    bool previewLayerEnable () { return m_previewLayerEnable; }
    bool drawLayerEnable () { return m_drawLayerEnable; }
    bool glyphRectLayerEnable () { return m_glyphRectLayerEnable; }
    bool baselineLayerEnable      () { return m_baselineLayerEnable; }
    bool bitmapRectLayerEnable ()  { return m_bitmapRectLayerEnable; } 

signals:    
    void templateColorChanged (const QColor &color);
    void templateBgColorChanged (const QColor &color);
    void previewColorChanged (const QColor &color);
    void previewBgColorChanged (const QColor &color);
    void drawColorChanged (const QColor &color);
    void drawBgColorChanged (const QColor &color);

    void fontMSBChanged(int value);

    void glyphSizeChanged(int value);
    void bitmapDimensionChanged(int value);
    void glyphFontChanged(const QFont &value);
    void glyphCharacterChanged(const QChar &value);
    void gridPaddingsChanged(const GridPaddings &value);
    void glyphPreviewMarginsChanged(const QMargins &value);
    void glyphWidgetMarginsChanged(const QMargins &value);

    void gridLayerEnableChanged(bool value);
    void templateLayerEnableChanged(bool value);
    void previewLayerEnableChanged(bool value);
    void drawLayerEnableChanged(bool value);
    void glyphRectLayerEnableChanged(bool value);
    void baselineLayerEnableChanged(bool value);
    void bitmapRectLayerEnableChanged(bool value);    

private:
    void saveAppSettings ();
    void restoreAppSettings ();

    QColor m_templateBgColor;
    QColor m_templateColor;
    QColor m_previewBgColor;
    QColor m_previewColor;
    QColor m_drawBgColor;
    QColor m_drawColor;

    bool m_gridLayerEnable;
    bool m_templateLayerEnable;
    bool m_previewLayerEnable;
    bool m_drawLayerEnable;
    bool m_glyphRectLayerEnable;
    bool m_baselineLayerEnable;
    bool m_bitmapRectLayerEnable;


    int m_bitmapDimension;
    int m_glyphSize;
    int m_fontMSB;
    QChar m_character;
    QFont m_font;

    QMargins m_glyphPreviewMargins;
    QMargins m_glyphWidgetMargins;

    GridPaddings m_gridPaddings;
};

#endif // COLORS_H
