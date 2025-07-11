#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QFile>

#include <windows.h>
#include <tchar.h>


class FontManager {
public:
    static FontManager& instance() {
        static FontManager instance;
        return instance;
    }

    QString findFontPath(const QString& family, const QString& style = "");
    void addFontDir(const QString& path); // Добавить кастомный путь для поиска

    // Запрещаем копирование и присваивание
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    bool GetSystemFontFilePath(const QString &fontName, QString &fontPath);
private:
    FontManager() = default; // Приватный конструктор
    ~FontManager() = default;

    QFontDatabase fontDb;
    QStringList customFontDirs;
    QMutex mutex; // Если нужна потокобезопасность

    const QString m_defaultFontsPath = "C:/Windows/Fonts/";
    const QString m_registerFontsPath = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
};
#endif // FONTMANAGER_H
