#include <QSettings>
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

#include "appsettings.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "profilemanager.h"

AppSettings::AppSettings(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
{
    setupValues();
}

AppSettings::~AppSettings ()
{
    saveAppSettings ();
}

void AppSettings::setupSignals ()
{
}

void AppSettings::setupValues()
{
    initSettings();
    initAppDataCatalog ();
    restoreAppSettings ();
    setupSignals ();
}

void AppSettings::initSettings ()
{
	m_values.insert("gridLayer", false);
	m_values.insert("templateLayer", false);
	m_values.insert("previewLayer", false);
	m_values.insert("drawLayer", false);
    m_values.insert("bitmapRectLayer", false);
	m_values.insert("glyphRectLayer", false);
	m_values.insert("baselineLayer", false);
    m_values.insert("leftlineLayer", false);
    m_values.insert("defaultProfileName", "-- Select Profile --");
    m_values.insert("defaultGlyphSize", 12);
    m_values.insert("defaultFontSize", 12);
    m_values.insert("defaultGridWidth", 12);
    m_values.insert("defaultGridHeight", 12);
    m_values.insert("defaultGridLeft", 3);
    m_values.insert("defaultGridTop", 3);
    m_values.insert("defaultGridRight", 3);
    m_values.insert("defaultGridBottom", 3);
    m_values.insert("glyphWidgetBitmapRectLayer", false);
    m_values.insert("glyphWidgetTemplateColor", QColor(0x00, 0x00, 0x55, 0x55));
    m_values.insert("glyphWidgetTemplateBgColor", QColor(0x00, 0x00, 0x00, 0xFF));        
    m_values.insert("glyphWidgetPreviewColor", QColor(0x66, 0x22, 0x00, 0x33));
    m_values.insert("glyphWidgetPreviewBgColor", QColor(0x66, 0x22, 0x00, 0x33));        
    m_values.insert("glyphWidgetDrawColor", QColor(0x33, 0x33, 0x33, 0xFF));
    m_values.insert("glyphWidgetDrawBgColor", QColor(0xFF, 0xFF, 0xFF, 0xFF));
    m_values.insert("glyphWidgetGridColor", QColor(0x33, 0x33, 0x33, 0xFF));
    m_values.insert("glyphWidgetGridBgColor", QColor(0xFF, 0xFF, 0xFF, 0xFF));
    m_values.insert("glyphWidgetBitmapRectColor", QColor(0x0, 0x0, 0x0, 0xFF));
    m_values.insert("glyphWidgetBitmapRectBgColor", QColor(0xFF, 0xFF, 0xFF, 0xFF));
    m_values.insert("glyphWidgetBaseLineColor", QColor(0x33, 0x90, 0x33, 0xFF));
    m_values.insert("glyphWidgetLeftLineColor", QColor(0x55, 0x55, 0xFF, 0xFF));
    m_values.insert("glyphWidgetGridLineWidth", 1);
    m_values.insert("glyphWidgetBaseLineWidth", 3);
    m_values.insert("glyphWidgetLeftLineWidth", 3);
    m_values.insert("glyphWidgetBaseLineStyle", static_cast<int>(Qt::SolidLine));
    m_values.insert("glyphWidgetLeftLineStyle", static_cast<int>(Qt::SolidLine));
    m_values.insert("glyphWidgetBitmapRectLineWidth", 2);
    m_values.insert("glyphWidgetBitmapRectLineStyle", static_cast<int>(Qt::DotLine));
    m_values.insert("glyphWidgetMargins", QVariant::fromValue(QMargins(50, 50, 50, 50)));
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

bool AppSettings::setValue(const QString &name, const QVariant &value)
{
    const auto &keys = m_values.keys();
    bool contains = keys.contains(name);
    m_values.insert(name, value);
    emit valueChanged(name, value);
    return contains;
}

QVariant AppSettings::value(const QString &name)
{
    auto it = m_values.find(name);
    if (it == m_values.end())
    {
        qDebug() << __FILE__ << __LINE__ << "Can't find value" << name;
        return QVariant();
    }

    // qDebug() << __FILE__ << __LINE__ << it.key() << it.value();

    return it.value();
}


const QString & AppSettings::appDataPath() const
{
    return m_appDataPath;
}

void AppSettings::clearAllSettings()
{
    QSettings settings(this);
    settings.clear();
}

void AppSettings::saveAppSettings()
{
    QSettings settings(this);

    settings.beginGroup("AppSettings");
    for (auto it = m_values.constBegin(); it != m_values.constEnd(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
    settings.sync();
}

void AppSettings::restoreAppSettings()
{
    QSettings settings(this);

    const QMap<QString, QVariant> &values = m_values;

    settings.beginGroup("AppSettings");
    for (auto it = m_values.constBegin(); it != m_values.constEnd(); ++it)
    {
#ifndef NOT_RESTORE_SETTINGS        
        m_values.insert(it.key(), settings.value(it.key()));
        // qDebug() << __FILE__ << __LINE__ << it.key() << it.value();
#endif        
    }
    settings.endGroup();
    // m_values.insert("glyphWidgetBaseLineColor", QColor(0x33, 0xFF, 0x33, 0xFF));
}
