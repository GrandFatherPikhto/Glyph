#include "appcontext.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_appSettings(nullptr)
{
    m_appSettings = new AppSettings(this);
    m_fontManager = new FontManager(this);
    m_glyphManager = new GlyphManager(m_fontManager, m_appSettings, this);
    m_unicodeMetadata = new UnicodeMetadata (this);

    restoreAppContext();

    setupSignals();
}

AppContext::~AppContext()
{
    saveAppContext();
}

void AppContext::setupSignals()
{
}

void AppContext::saveAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");
    

    // settings.setValue("glyph/size", m_glyphSize);
    // settings.setValue("glyph/bitmapDimension", m_bitmapDimension);
    // settings.setValue("glyph/font", m_fontManager->glyphFont());
    // settings.setValue("glyph/character", m_character.unicode());

    // settings.setValue("glyph/leftGridCells", m_leftGridCells);
    // settings.setValue("glyph/bottomGridCells", m_bottomGridCells);

    settings.setValue("glyph/fontMSB", m_fontMSB);

    // settings.setValue("glyph/margins/left", m_margins.left());
    // settings.setValue("glyph/margins/top", m_margins.top());
    // settings.setValue("glyph/margins/right", m_margins.right());
    // settings.setValue("glyph/margins/bottom", m_margins.bottom());

    settings.endGroup();
}

void AppContext::restoreAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");

    // m_glyphSize = settings.value("glyph/size").toInt();
    // m_bitmapDimension = settings.value("glyph/bitmapDimension").toInt();
    
    // int unicode = settings.value("glyph/character").toInt();
    // if (unicode >= 0)
    // {
    //     m_character = QChar(unicode);
    // }

    // m_leftGridCells = settings.value("glyph/leftGridCells", 0).toInt();
    // m_bottomGridCells = settings.value("glyph/bottomGridCells", 0).toInt();
    m_fontMSB = settings.value("glyph/fontMSB", -1).toInt();
    // m_margins = QMargins(
    //     settings.value("glyph/margins/left", 50).toInt(),
    //     settings.value("glyph/margins/top", 50).toInt(),
    //     settings.value("glyph/margins/right", 50).toInt(),
    //     settings.value("glyph/margins/bottom", 50).toInt()
    //     );
    settings.endGroup();
}

