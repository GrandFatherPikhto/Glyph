#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include "appsettings.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "profilemanager.h"

AppSettings::AppSettings(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_fontManager(nullptr)
{
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &AppSettings::setupValues);
}

AppSettings::~AppSettings ()
{
    // qDebug() << __FILE__ << __LINE__ << "Destroy AppSettings class";
    saveAppSettings ();
}

void AppSettings::setupSignals ()
{
}

void AppSettings::setupValues()
{
    Q_ASSERT(m_appContext->fontManager() != nullptr && m_appContext->profileManager() != nullptr);
    m_fontManager = m_appContext->fontManager();
    m_profileManager = m_appContext->profileManager();

    initAppDataCatalog ();
    restoreAppSettings ();
    
    setupSignals ();
}

const QString & AppSettings::initAppDataCatalog()
{
    // Получение пути к AppData/Local/<AppName>
    m_appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    // qDebug() << m_appDataPath;
    // Создание каталога, если его нет
    QDir dir(m_appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return m_appDataPath;
}

const QString & AppSettings::appDataPath() const
{
    return m_appDataPath;
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
    settings.endGroup();

    settings.beginGroup("GlyphWidget");
    settings.setValue("widgetMargins", QVariant::fromValue(m_glyphWidgetMargins));
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

    settings.sync();
}

void AppSettings::restoreAppSettings()
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
    settings.endGroup();

    settings.beginGroup("GlyphWidget");
    m_glyphWidgetMargins = settings.value("widgetMargins", QVariant::fromValue(QMargins(50,50,50,50))).value<QMargins>();
    settings.endGroup();

    settings.beginGroup("GlyphPrevew");
    m_glyphPreviewMargins = settings.value("previewMargins", QVariant::fromValue(QMargins(3,3,3,3))).value<QMargins>();
    settings.endGroup();

    settings.beginGroup("MainToolbar");
    m_gridLayerEnable = settings.value("gridLayerEnable").toBool();
    m_templateLayerEnable = settings.value("templateLayerEnable").toBool();
    m_previewLayerEnable = settings.value("previewLayerEnable").toBool();
    m_drawLayerEnable = settings.value("drawLayerEnable").toBool();
    m_glyphRectLayerEnable = settings.value("glyphRectLayerEnable").toBool();
    m_baselineLayerEnable = settings.value("baselineLayerEnable").toBool();
    m_bitmapRectLayerEnable = settings.value("bitmapRectLayerEnable").toBool();
    settings.endGroup();

    settings.beginGroup("Projects");
    m_defaultProjectPath = settings.value("defaultProjectPath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    settings.endGroup();
}

