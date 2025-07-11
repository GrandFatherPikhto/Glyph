#include "fontmanager.h"

// FontManager::FontManager instance()
// {
//     static FontManager instance;
//     return instance;
// }

// FontManager.cpp
QString FontManager::findFontPath(const QString& family, const QString& style) {
    QMutexLocker locker(&mutex); // Если нужно защитить доступ

    // Пример: поиск в кастомных директориях
    for (const auto& dir : customFontDirs) {
        QString path = dir + "/" + family + ".ttf";
        if (QFile::exists(path)) {
            return path;
        }
    }

    // Стандартный поиск через QFontDatabase
    if (fontDb.families().contains(family)) {
        // Qt не даёт путь, но можно попробовать угадать (см. предыдущий ответ)
        return QString();
    }

    return QString();
}


