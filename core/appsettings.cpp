#include "appsettings.h"

AppSettings::AppSettings(FontManager *fontManager, QObject *parent)
    : QObject{parent}
    , m_fontManager(fontManager)
    , m_templateColor(QColor(0x00, 0x00, 0x55, 0x55))
    , m_templateBgColor(Qt::white)
    , m_previewColor(QColor(0x66, 0x22, 0x00, 0x33))
    , m_previewBgColor(Qt::white)
    , m_drawColor(Qt::black)
    , m_drawBgColor(Qt::white)
    , m_fontPath(QString(""))
    , m_font(QFont())
    , m_glyphSize(-1)
    , m_bitmapDimension(-1)
    , m_character(QChar())
{

}

AppSettings::~AppSettings()
{

}

void AppSettings::saveApplicationSettings()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");
    
    settings.setValue("glyph/template/color", m_templateColor);
    settings.setValue("glyph/template/bgColor", m_templateBgColor);
    settings.setValue("glyph/preview/color", m_previewColor);
    settings.setValue("glyph/preview/bgColor", m_previewBgColor);
    settings.setValue("glyph/draw/color", m_drawColor);
    settings.setValue("glyph/draw/bgColor", m_drawBgColor);
    settings.setValue("glyph/size", m_glyphSize);
    settings.setValue("glyph/bitmapDimension", m_bitmapDimension);
    settings.setValue("glyph/font", m_font);
    settings.setValue("glyph/fontPath", m_fontPath);
    settings.setValue("glyph/character", m_character.unicode());

    settings.endGroup();
}

void AppSettings::restoreApplicationSettings()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");

    m_templateColor = settings.value("glyph/template/color").value<QColor>();
    m_templateBgColor = settings.value("glyph/template/bgColor").value<QColor>();
    m_previewColor = settings.value("glyph/preview/color").value<QColor>();
    m_previewBgColor = settings.value("glyph/preview/bgColor").value<QColor>();
    m_drawColor = settings.value("glyph/draw/color").value<QColor>();
    m_drawBgColor = settings.value("glyph/draw/bgColor").value<QColor>();
    m_glyphSize = settings.value("glyph/size").toInt();
    m_bitmapDimension = settings.value("glyph/bitmapDimension").toInt();
    m_fontPath = settings.value("glyph/fontPath").toString();
    int unicode = settings.value("glyph/character").toInt();
    if (unicode >= 0)
    {
        m_character = QChar(unicode);
    }

    settings.endGroup();
}