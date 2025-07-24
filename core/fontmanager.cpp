#include <QDebug>

#include "fontmanager.h"

FontManager::FontManager(QObject *parent)
    : QObject(parent)
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_fontPath(QString())
{
    initDefault();
    if (FT_Init_FreeType(&m_ftLibrary)) {
        fprintf(stderr, "Could not init FreeType library\n");
        // return 1;
    }
}

FontManager::~FontManager()
{
    // Очистка
    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
    }

    if (m_ftLibrary)
    {
        FT_Done_FreeType(m_ftLibrary);
    }
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

QString FontManager::findFontPath(const QFont &font)
{
    return findFontPath(font.family());
}

QString FontManager::findFontPath(const QString& fontFamily, const QString& fontStyle) {
    m_fontFamily = fontFamily;
    m_fontStyle = fontStyle;

#if defined(Q_OS_WIN)
    return getRegisterFontFilePath();
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MACOS)
#endif

    return QString();
}

#if defined(Q_OS_WIN)
QString FontManager::getRegisterFontFilePath() 
{
    HKEY hKey;
    for (const QString &registerPath : m_registerFontPath) {
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

void FontManager::addFontDir(const QString &newFontDir) 
{
    m_fontDirectory.append(newFontDir);
}


const QVector<quint64> & FontManager::listSupportedChars(const QFont &font)
{
    FT_ULong charcode;
    FT_UInt gindex;

    QString fontPath = findFontPath(font);

    // Загрузка шрифта
    if (FT_New_Face(m_ftLibrary, fontPath.toStdString().c_str(), 0, &m_ftFace)) {
        fprintf(stderr, "Could not open font\n");
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = 0;
        // return 1;
    }

    // Установка размера шрифта (опционально)
    FT_Set_Pixel_Sizes(m_ftFace, 0, 16);

    // Получаем первый символ в charmap
    charcode = FT_Get_First_Char(m_ftFace, &gindex);
    m_supportedChars.clear();

    while (gindex != 0) {
        // Выводим код символа (можете преобразовать в Unicode при необходимости)
        // printf("Char code: %lu, Glyph index: %u\n", charcode, gindex);

        // Получаем следующий символ
        charcode = FT_Get_Next_Char(m_ftFace, charcode, &gindex);
        m_supportedChars.append(charcode);
    }

    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }

    return m_supportedChars;
}
