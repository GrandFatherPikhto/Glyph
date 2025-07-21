#include <QDebug>

#include "fontmanager.h"

FontManager::FontManager(QObject *parent)
    : QObject(parent)
{
    initDefault();
}

FontManager::~FontManager()
{

}

void FontManager::initDefault ()
{
#if defined(Q_OS_WIN)
    // Windows-специфичный код
    qDebug() << "Running on Windows";
    m_fontDirectory.append("C:/Windows/Fonts/");
    m_registerFontPath.append("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
#elif defined(Q_OS_LINUX)
    // Linux-специфичный код
    qDebug() << "Running on Linux";
#elif defined(Q_OS_MACOS)
    // macOS-специфичный код
    qDebug() << "Running on macOS";
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
        qDebug() << registerPath;
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
