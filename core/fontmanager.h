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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

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

    const QVector<quint64> & listSupportedChars (const QFont &font);

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
    QString m_fontPath;

    FT_Library m_ftLibrary;
    FT_Face m_ftFace;

    QVector<quint64> m_supportedChars;

};
#endif // FONTMANAGER_H
