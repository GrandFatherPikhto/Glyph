#include "fontmanager.h"
#include "appcontext.h"

FontManager::FontManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_fontPath(QString())
{
    QObject::connect(m_appContext, &AppContext::valuesInited, this, &FontManager::setupValues);
}

FontManager::~FontManager()
{

}

void FontManager::setupValues ()
{
    initContext ();
    setupSignals ();
}

void FontManager::setupSignals ()
{

}

bool FontManager::loadFont(const QFont &font)
{
    if (font == QFont())
        return false;

    m_font = font;
    m_fontFamily = font.family();

    return getRegisterFontFilePath();
}

void FontManager::initContext()
{
#if defined(Q_OS_WIN)
        // Windows-специфичный код
        // qDebug() << "Running on Windows";
        m_fontDirectories.append("C:/Windows/Fonts/");
        m_fontRegisterPaths.append("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
#elif defined(Q_OS_LINUX)
        // Linux-специфичный код
        // qDebug() << "Running on Linux";
#elif defined(Q_OS_MACOS)
        // macOS-специфичный код
        // qDebug() << "Running on macOS";
#endif
}

#if defined(Q_OS_WIN)
bool FontManager::getRegisterFontFilePath()
{
    HKEY hKey;
    for (const QString &registerPath : std::as_const(m_fontRegisterPaths)) {
        // qDebug() << registerPath;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registerPath.toStdWString().c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            continue;
        }

        QString fontPath = getFontPathOverRegisterKey(hKey);
        RegCloseKey(hKey);
        if (!fontPath.isEmpty())
        {
            m_fontPath = fontPath;
            return true;
        }
    }

    return false;
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
            foreach(const QString &fontDirectory, m_fontDirectories)
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

const QString & FontManager::fontPath()
{
    return m_fontPath;
}

