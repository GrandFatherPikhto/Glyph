#include <QStandardPaths>

#include "appcontext.h"
#include "appsettings.h"

AppSettings::AppSettings(AppContext *appContext) 
    : QObject{appContext}
    , m_appContext(appContext)
    , m_templateColor(QColor(0x00, 0x00, 0x55, 0x55))
    , m_templateBgColor(Qt::white)
    , m_previewColor(QColor(0x66, 0x22, 0x00, 0x33))
    , m_previewBgColor(Qt::white)
    , m_drawColor(Qt::gray)
    , m_drawBgColor(Qt::white)
    , m_glyphSize(-1)
    , m_bitmapDimension(-1)
    , m_character(QChar('a'))
    , m_font(QFont("Arial"))
    , m_gridPaddings(GridPaddings(0,0,0,0))
    , m_glyphPreviewMargins(QMargins(0,0,0,0))
    , m_glyphWidgetMargins(QMargins(50,50,50,50))
    , m_gridLayerEnable(false)
    , m_templateLayerEnable(false)
    , m_previewLayerEnable(false)
    , m_drawLayerEnable(false)
    , m_glyphRectLayerEnable(false)
    , m_baselineLayerEnable(false)
    , m_bitmapRectLayerEnable(false)
    , m_fontMSB(-1)
{
    restoreAppSettings();
}

AppSettings::~AppSettings()
{
    saveAppSettings();
}

void AppSettings::saveAppSettings()
{
    QSettings settings(this);

    settings.beginGroup("Glyph");
    settings.setValue("templateColor", m_templateColor);
    settings.setValue("templateBgColor", m_templateBgColor);
    settings.setValue("previewColor", m_previewColor);
    settings.setValue("previewBgColor", m_previewBgColor);
    settings.setValue("drawColor", m_drawColor);
    settings.setValue("drawBgColor", m_drawBgColor);
    settings.setValue("size", m_glyphSize);
    settings.setValue("bitmapSettings", m_bitmapDimension);
    settings.setValue("character", m_character);
    settings.setValue("font", m_font);
    settings.endGroup();

    settings.beginGroup("GlyphWidget");
    settings.setValue("widgetMargins", QVariant::fromValue(m_glyphWidgetMargins));
    settings.setValue("gridPaddings", QVariant::fromValue(m_gridPaddings));
    settings.endGroup();

    settings.beginGroup("GlyphPreview");
    settings.setValue("previewMargins", QVariant::fromValue(m_glyphPreviewMargins));
    settings.endGroup();

    settings.beginGroup("MainToolbar");
    settings.setValue("gridLayerEnable", m_gridLayerEnable);
    settings.setValue("templateLayerEnable", m_templateLayerEnable);
    settings.setValue("previewLayerEnable", m_previewLayerEnable);
    settings.setValue("drawLayerEnable", m_drawLayerEnable);
    settings.setValue("glyphRectLayerEnable", m_glyphRectLayerEnable);
    settings.setValue("baselineLayerEnable", m_baselineLayerEnable);
    settings.setValue("bitmapRectLayerEnable", m_bitmapRectLayerEnable);
    settings.endGroup();

    settings.beginGroup("Projects");
    settings.setValue("defaultProjectPath", m_defaultProjectPath);
    settings.endGroup();

    settings.sync(); // Важно: синхронизируем с диском
}

void AppSettings::restoreAppSettings ()
{
    QSettings settings(this);

    settings.beginGroup("Glyph");
    QColor templateColor = settings.value("templateColor", QColor(0x00, 0x00, 0x55, 0x55)).value<QColor>();
    if (templateColor.isValid())
        m_templateColor = templateColor;

    QColor templateBgColor = settings.value("templateBgColor", QColor(Qt::white)).value<QColor>();
    if (templateBgColor.isValid())
        m_templateBgColor = templateBgColor;

    QColor previewColor = settings.value("previewColor", QColor(0x66, 0x22, 0x00, 0x33)).value<QColor>();
    if (previewColor.isValid())
       m_previewColor = previewColor;

    QColor previewBgColor = settings.value("previewBgColor", QColor(Qt::white)).value<QColor>();
    if (previewBgColor.isValid())
        m_previewBgColor = previewBgColor;

    QColor drawColor = settings.value("drawColor", QColor(Qt::gray)).value<QColor>();
    if (drawColor.isValid())
        m_drawColor = drawColor;

    QColor drawBgColor = settings.value("drawBgColor", QColor(Qt::white)).value<QColor>();
    if (drawBgColor.isValid())
        m_drawBgColor = drawBgColor;

    m_glyphSize = settings.value("size", 10).toInt();
    m_bitmapDimension = settings.value("bitmapSize", 12).toInt();

    m_character = settings.value("character", QChar('a')).value<QChar>();
    m_font = settings.value("font", QFont("Arial")).value<QFont>();
    settings.endGroup();

    settings.beginGroup("GlyphWidget");
    m_gridPaddings = settings.value("gridPaddings", QVariant::fromValue(GridPaddings{0,0,0,0})).value<GridPaddings>();
    m_glyphWidgetMargins = settings.value("widgetMargins", QVariant::fromValue(QMargins(50,50,50,50))).value<QMargins>();
    settings.endGroup();

    settings.beginGroup("GlyphPrevew");
    m_glyphPreviewMargins = settings.value("previewMargins", QVariant::fromValue(QMargins(3,3,3,3))).value<QMargins>();

    settings.endGroup();

    settings.beginGroup("MainToolbar");
    m_gridLayerEnable = settings.value("glyph/gridLayerEnable").toBool();
    m_templateLayerEnable = settings.value("glyph/templateLayerEnable").toBool();
    m_previewLayerEnable = settings.value("glyph/previewLayerEnable").toBool();
    m_drawLayerEnable = settings.value("glyph/drawLayerEnable").toBool();
    m_glyphRectLayerEnable = settings.value("glyph/glyphRectLayerEnable").toBool();
    m_baselineLayerEnable = settings.value("glyph/baselineLayerEnable").toBool();
    m_bitmapRectLayerEnable = settings.value("glyph/bitmapRectLayerEnable").toBool();
    settings.endGroup();

    settings.beginGroup("Projects");
    m_defaultProjectPath = settings.value("defaultProjectPath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    settings.endGroup();
}
