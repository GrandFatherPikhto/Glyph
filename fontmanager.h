#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QFile>

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

private:
    FontManager() = default; // Приватный конструктор
    ~FontManager() = default;

    QFontDatabase fontDb;
    QStringList customFontDirs;
    QMutex mutex; // Если нужна потокобезопасность
};
#endif // FONTMANAGER_H
