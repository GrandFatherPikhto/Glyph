#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QtGlobal>

#include <windows.h>
#include <tchar.h>


class FontManager : public QObject {
    Q_OBJECT
public:
    FontManager(QObject *parent = nullptr);
    ~FontManager();

    QString findFontPath(const QFont &font);
    QString findFontPath(const QString& family, const QString& style = "");
    void addFontDir(const QString& path); // Добавить кастомный путь для поиска

    void addFontPath(const QString &fontPath)
    {
        m_fontDirectory.append(fontPath);
    }

    void addRegisterFontPath(const QString &registerFontPath)
    {
        m_registerFontPath.append(registerFontPath);
    }

private:
#if defined(Q_OS_WIN)
    QString getRegisterFontFilePath();
    QString getFontPathOverRegisterKey (HKEY &hKey);
#endif // Q_OS_WIN

    void initDefault ();

    QStringList customFontDirs;

    QStringList m_fontDirectory;
    QStringList m_registerFontPath;

    QString m_fontFamily;
    QString m_fontStyle;
};
#endif // FONTMANAGER_H
