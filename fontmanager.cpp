#include <QDebug>

#include "fontmanager.h"

// FontManager::FontManager instance()
// {
//     static FontManager instance;
//     return instance;
// }

// FontManager.cpp
QString FontManager::findFontPath(const QString& family, const QString& style) {
    QMutexLocker locker(&mutex); // Если нужно защитить доступ
    // qDebug() << __FILE__ << "contains" << family << fontDb.families().contains(family);

    // Пример: поиск в кастомных директориях
    for (const auto& dir : customFontDirs) {
        QString path = dir + "/" + family + ".ttf";
        if (QFile::exists(path)) {
            return path;
        }
    }

    // Стандартный поиск через QFontDatabase
    if (fontDb.families().contains(family)) {
        QString fontPath;
        if (GetSystemFontFilePath(family, fontPath))
        {
            return fontPath;
        }

        return QString();
    }

    return QString();
}


bool FontManager::GetSystemFontFilePath(const QString &fontName, QString &fontPath) {
    HKEY hKey;
    uint32_t pathSize = 0;
    // TCHAR regPath[] = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
    
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_registerFontsPath.toStdWString().c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

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
        fontName.toWCharArray(tchFontName);
        // Проверяем, содержит ли имя шрифта искомое название
        if (_tcsstr(valueName, tchFontName) != NULL) {
            fontPath = QString("%1%2").arg(m_defaultFontsPath).arg(valueData);
            // _stprintf_s(tchFontPath, _T("C:\\Windows\\Fonts\\%s"), valueData);
            // fontPath = QString::fromStdWString(tchFontPath);
            found = true;
            break;
        }
        index++;
    }


    RegCloseKey(hKey);



    return found;
}

void FontManager::addFontDir(const QString &newFontDir) 
{

}
