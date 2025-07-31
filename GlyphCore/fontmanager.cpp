#include <QDebug>

#include "fontmanager.h"
#include "appcontext.h"

FontManager::FontManager(AppContext *appContext)
    : QObject(appContext)
    , m_appContext(appContext)
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_fontPath(QString())
    , m_fontMSBFilter(0)
    , m_fontSize(-1)
    , m_fromFilter(-1)
    , m_toFilter(-1)
{
    initDefault();
    if (FT_Init_FreeType(&m_ftLibrary)) {
        fprintf(stderr, "Could not init FreeType library\n");
    }
    restoreFontManagerSettings ();
}

FontManager::~FontManager()
{
    saveFontManagerSettings ();
    // Очистка
    releaseFontFace ();
    releaseLibrary  ();
}

void FontManager::initDefault ()
{
#if defined(Q_OS_WIN)
    // Windows-специфичный код
    // qDebug() << "Running on Windows";
    m_fontDirectory.append("C:/Windows/Fonts/");
    m_registerFontPath.append("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
#elif defined(Q_OS_LINUX)
    // Linux-специфичный код
    // qDebug() << "Running on Linux";
#elif defined(Q_OS_MACOS)
    // macOS-специфичный код
    // qDebug() << "Running on macOS";
#endif    
}

bool FontManager::setGlyphFont(const QFont &font) 
{
    if (font == QFont() || (m_font != QFont() && m_font.family() == font.family()))
        return false;

    m_font = font;
    m_fontFamily = font.family();

    if (m_fontFamily.isEmpty())
        return false;


#if defined(Q_OS_WIN)
    m_fontPath = getRegisterFontFilePath();
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MACOS)
#endif

    initFontContext();
    filterCharacters ();

    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << font << m_supportedChars.size() << m_fontPath << m_filteredChars.size() << ", MSB: " << m_fontMSBFilter;

    emit glyphFontChanged (m_font);

    return true;
}

#if defined(Q_OS_WIN)
QString FontManager::getRegisterFontFilePath() 
{
    HKEY hKey;
    for (const QString &registerPath : std::as_const(m_registerFontPath)) {
        // qDebug() << registerPath;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registerPath.toStdWString().c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            continue;
        }

        QString fontPath = getFontPathOverRegisterKey(hKey);            
        RegCloseKey(hKey);
        if (!fontPath.isEmpty())
            return fontPath;
    }

    return QString();
}
#endif // Q_OS_WIN

#if defined(Q_OS_WIN)
QString FontManager::getFontPathOverRegisterKey(HKEY &hKey)
{
    uint32_t pathSize = 0;
    TCHAR valueName[256];
    TCHAR valueData[MAX_PATH];
    DWORD valueNameSize, valueDataSize, type;
    DWORD index = 0;
    bool found = false;
    while (true) {
        valueNameSize = sizeof(valueName) / sizeof(TCHAR);
        valueDataSize = sizeof(valueData);
        
        if (RegEnumValue(hKey, index, valueName, &valueNameSize, NULL,
                        &type, (LPBYTE)valueData, &valueDataSize) != ERROR_SUCCESS) {
            break;
        }

        TCHAR tchFontName[MAX_PATH] = {0};
        TCHAR tchFontPath[MAX_PATH] = {0};
        m_fontFamily.toWCharArray(tchFontName);
        // Проверяем, содержит ли имя шрифта искомое название
        if (_tcsstr(valueName, tchFontName) != NULL) {
            foreach(const QString &fontDirectory, m_fontDirectory)
            {
                QString fontPath = QString("%1%2").arg(fontDirectory).arg(valueData);
                if (QFile::exists(fontPath))
                {
                    return fontPath;
                }
            }
        }
        index++;
    }

    return QString();
}
#endif // Q_OS_WIN

void FontManager::releaseFontFace ()
{
    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

void FontManager::releaseLibrary ()
{
    if (m_ftLibrary)
    {
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = 0;
    }
}

void FontManager::initFontContext()
{
    FT_ULong charcode;
    FT_UInt gindex;

    if (m_fontPath.isEmpty())
        return;

    releaseFontFace();

    // Загрузка шрифта
    if (FT_New_Face(m_ftLibrary, m_fontPath.toStdString().c_str(), 0, &m_ftFace)) {
        fprintf(stderr, "Could not open font\n");
        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ <<"Could not open font" << m_font << m_fontPath;
        // FT_Done_FreeType(m_ftLibrary);
        releaseFontFace();
    }

    // Установка размера шрифта (опционально)
    FT_Set_Pixel_Sizes(m_ftFace, 0, 16);

    // Получаем первый символ в charmap
    charcode = FT_Get_First_Char(m_ftFace, &gindex);
    m_supportedChars.clear ();
    m_fontCategories.clear ();
    m_fontDecompositions.clear ();
    m_fontScripts.clear ();

    while (gindex != 0) {
        // Выводим код символа (можете преобразовать в Unicode при необходимости)
        // printf("Char code: %lu, Glyph index: %u\n", charcode, gindex);

        // Получаем следующий символ
        charcode = FT_Get_Next_Char(m_ftFace, charcode, &gindex);
        if (charcode >= 0xFFFFFFFF)
             continue;

        QChar ch(static_cast<quint32>(charcode));
        m_supportedChars.append(ch);

        if (!m_fontCategories.contains(ch.category()))
        {
            m_fontCategories.append(ch.category());
        }

        if (!m_fontScripts.contains(ch.script()))
        {
            m_fontScripts.append(ch.script());
        }

        if (!m_fontDecompositions.contains(ch.decompositionTag()))
        {
            m_fontDecompositions.append(ch.decompositionTag());
        }
    }

    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

void FontManager::setScriptFilter(const QVector<QChar::Script> &filter)
{
    m_fontScriptsFilter = filter;
    filterCharacters ();
}

void FontManager::setCategoryFilter(const QVector<QChar::Category> &filter)
{
    m_fontCategoriesFilter = filter;
    filterCharacters ();
}

void FontManager::setDecompositionFilter(const QVector<QChar::Decomposition> &filter)
{
    m_fontDecompositionsFilter = filter;
    filterCharacters ();
}

void FontManager::setCharactersFilter(const QString &filter)
{
    m_charactersFilter = filter;
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_charactersFilter;
    filterCharacters ();
}

void FontManager::setFontMSBFilter (int filter)
{
    m_fontMSBFilter = filter;
    filterCharacters ();
}

void FontManager::setFromToFilter(int msb, int from, int to)
{
    if (msb >= 0 && from >= 0 && to >= 0)
    {
        m_fontMSBFilter = msb;
        m_fromFilter = (from < to) ? from : to;
        m_toFilter = (from > to) ? from : to;
        filterCharacters ();
    }
}

void FontManager::filterCharacters ()
{
    m_filteredChars.clear();
    for (const auto &character : std::as_const(m_supportedChars))
    {
        if (
            (m_fontMSBFilter < 0 || character.row() == m_fontMSBFilter)
            && (m_fontCategoriesFilter.isEmpty() || m_fontCategoriesFilter.contains(character.category()))
            && (m_fontScriptsFilter.isEmpty() || m_fontScriptsFilter.contains(character.script()))
            && (m_fontDecompositionsFilter.isEmpty() || m_fontDecompositionsFilter.contains(character.decompositionTag()))
            && (m_charactersFilter.isEmpty() || m_charactersFilter.contains(character))
            && ((m_fromFilter < 0 || m_toFilter < 0 || m_fontMSBFilter < 0) ||
                ((character.unicode() >= ((m_fontMSBFilter << 8) & 0xFF00 | (m_fromFilter & 0x00FF))) &&
                    (character.unicode() <= ((m_fontMSBFilter << 8) & 0xFF00 | (m_toFilter & 0x00FF))))) 
        )
        {
            m_filteredChars.append(character);
        }
    }

    emit filteredCharactersChanged (m_filteredChars);
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_filteredChars.size();
}

void FontManager::saveFontManagerSettings ()
{
    QSettings settings;

    settings.beginGroup("FontManager");
    settings.setValue("font", m_font);
    settings.setValue("size", m_fontSize);
    settings.setValue("filter/MSB", m_fontMSBFilter);
    settings.setValue("filter/from", m_fromFilter);
    settings.setValue("filter/to", m_toFilter);
    settings.setValue("filter/characters", m_charactersFilter);
    settings.endGroup();
}

void FontManager::restoreFontManagerSettings ()
{
    QSettings settings;
    settings.beginGroup("FontManager");
    m_fontSize = settings.value("size", 14).toInt();
    QFont font = settings.value("font").value<QFont>();
    setGlyphFont(font);
    m_fontMSBFilter = settings.value("filter/MSB", 0).toInt();
    m_fromFilter = settings.value("filter/from", -1).toInt();
    m_toFilter = settings.value("filter/to", -1).toInt();
    m_charactersFilter = settings.value("filter/characters", "").toString();
    settings.endGroup();
}
