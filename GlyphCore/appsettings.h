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

    // Getters
    const QString & defaultProjectPath() { return m_defaultProjectPath; }

    const QColor & templateColor() { return m_templateColor; }
    const QColor & templateBgColor() { return m_templateBgColor; }
    const QColor & previewColor () { return m_previewColor; }
    const QColor & previewBgColor () { return m_previewBgColor; }
    const QColor & drawColor () { return m_drawColor; }
    const QColor & drawBgColor () { return m_drawBgColor; }

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
};

#endif // APPSETTINGS_H_
