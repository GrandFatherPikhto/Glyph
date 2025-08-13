#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <QObject>
#include <QFont>
#include <QString>
#include <QColor>
#include <QPoint>
#include <QMargins>
#include <QRect>
#include <QSharedPointer>

#include "GlyphCore_global.h"

class AppContext;
class FontManager;
class ProfileManager;

class GLYPHCORE_EXPORT AppSettings : public QObject {
    Q_OBJECT
public:
    explicit AppSettings(AppContext *appContext);
    ~AppSettings ();

    const QString & appDataPath () const;

    // Setters
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

    void setGridColor (const QColor &color)
    {
        if (m_gridColor != color)
        {
            m_gridColor = color;
            emit gridColorChanged(m_gridColor);
        }
    }

    void setGridBgColor (const QColor &color)
    {
        if (m_gridBgColor != color)
        {
            m_gridBgColor = color;
            emit gridBgColorChanged(m_gridBgColor);
        }
    }

    void setBitmapRectColor(const QColor &color)
    {
        if (m_bitmapRectColor != color)
        {
            m_bitmapRectColor = color;
            emit bitmapRectColorChanged(m_bitmapRectColor);
        }
    }

    void setBitmapRectBgColor(const QColor &color)
    {
        if (m_bitmapRectBgColor != color)
        {
            m_bitmapRectBgColor = color;
            emit bitmapRectBgColorChanged(m_bitmapRectBgColor);
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

    void setBitmapRectLayerEnable (bool value) {
        if (m_bitmapRectLayerEnable != value)
        {
            m_bitmapRectLayerEnable = value;
            emit bitmapRectLayerEnableChanged(m_bitmapRectLayerEnable);
        }
    }

    void setDefaultProjectPath (const QString &value) { 
        m_defaultProjectPath = value; 
    }

    void setWidgetGridLineWidth(int value) { 
        if (m_widgetGridLineWidth != value)
        {
            m_widgetGridLineWidth = value;
            emit widgetGridLineWidthChanged(m_widgetGridLineWidth);
        }
    }

    void setWidgetBitmapRectLineWidth(int value) { 
        if (m_widgetBitmapRectLineWidth != value)
        {
            m_widgetBitmapRectLineWidth = value;
            emit widgetBitmapRectLineWidthChanged(m_widgetBitmapRectLineWidth);
        }
    }

    // Getters
    const QString & defaultProjectPath() { return m_defaultProjectPath; }

    const QColor & templateColor() const { return m_templateColor; }
    const QColor & templateBgColor() const { return m_templateBgColor; }
    const QColor & previewColor () const { return m_previewColor; }
    const QColor & previewBgColor () const { return m_previewBgColor; }
    const QColor & drawColor () const { return m_drawColor; }
    const QColor & drawBgColor () const { return m_drawBgColor; }
    const QColor & gridColor() const { return m_gridColor; }
    const QColor & gridBgColor() const { return m_gridBgColor; }
    const QColor & bitmapRectColor() const { return m_bitmapRectColor; }
    const QColor & bitmapRectBgColor() const { return m_bitmapRectBgColor; }

    const QMargins & glyphPreviewMargins () { return m_glyphPreviewMargins; }
    const QMargins & glyphWidgetMargins () { return m_glyphWidgetMargins; }

    bool gridLayerEnable () { return m_gridLayerEnable; }
    bool templateLayerEnable () { return m_templateLayerEnable; }
    bool previewLayerEnable () { return m_previewLayerEnable; }
    bool drawLayerEnable () { return m_drawLayerEnable; }
    bool glyphRectLayerEnable () { return m_glyphRectLayerEnable; }
    bool baselineLayerEnable      () { return m_baselineLayerEnable; }
    bool bitmapRectLayerEnable ()  { return m_bitmapRectLayerEnable; }

    int widgetGridLineWidth() { return m_widgetGridLineWidth; }
    int widgetBitmapRectLineWidth() { return m_widgetBitmapRectLineWidth; }

signals:
    void templateColorChanged (const QColor &color);
    void templateBgColorChanged (const QColor &color);
    void previewColorChanged (const QColor &color);
    void previewBgColorChanged (const QColor &color);
    void drawColorChanged (const QColor &color);
    void drawBgColorChanged (const QColor &color);
    void gridColorChanged(const QColor &color);
    void gridBgColorChanged(const QColor &color);
    void bitmapRectColorChanged(const QColor &color);
    void bitmapRectBgColorChanged(const QColor &color);

    void glyphPreviewMarginsChanged(const QMargins &value);
    void glyphWidgetMarginsChanged(const QMargins &value);

    void gridLayerEnableChanged(bool value);
    void templateLayerEnableChanged(bool value);
    void previewLayerEnableChanged(bool value);
    void drawLayerEnableChanged(bool value);
    void glyphRectLayerEnableChanged(bool value);
    void baselineLayerEnableChanged(bool value);
    void bitmapRectLayerEnableChanged(bool value);
    
    void widgetGridLineWidthChanged(int value);
    void widgetBitmapRectLineWidthChanged(int value);

private:
    void setupValues();
    void setupSignals();
    void saveAppSettings();
    void restoreAppSettings();
    const QString & initAppDataCatalog();


    AppContext *m_appContext;
    FontManager *m_fontManager;
    ProfileManager *m_profileManager;

    QString m_appDataPath;

    QColor m_templateBgColor;
    QColor m_templateColor;
    QColor m_previewBgColor;
    QColor m_previewColor;
    QColor m_drawBgColor;
    QColor m_drawColor;
    QColor m_gridColor;
    QColor m_gridBgColor;
    QColor m_bitmapRectColor;
    QColor m_bitmapRectBgColor;

    bool m_gridLayerEnable;
    bool m_templateLayerEnable;
    bool m_previewLayerEnable;
    bool m_drawLayerEnable;
    bool m_glyphRectLayerEnable;
    bool m_baselineLayerEnable;
    bool m_bitmapRectLayerEnable;

    QMargins m_glyphPreviewMargins;
    QMargins m_glyphWidgetMargins;

    QString m_defaultProjectPath;

    int m_widgetGridLineWidth;
    int m_widgetBitmapRectLineWidth;
};

#endif // APPSETTINGS_H_
