#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QtGlobal>
#include <QSettings>
#include <QVector>

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

    bool setGlyphFont(const QFont &font);
    void setFontSize(int value) { m_fontSize = value; }
    int fontSize () { return m_fontSize; }

    const QString & glyphFontPath() { return m_fontPath; }

    void addFontsDirectory(const QString &fontPath)
    {
        m_fontDirectory.append(fontPath);
    }

    void addRegisterFontPath(const QString &registerFontPath)
    {
        m_registerFontPath.append(registerFontPath);
    }

    void setScriptFilter(const QVector<QChar::Script> &filter);
    void setCategoryFilter(const QVector<QChar::Category> &filter);
    void setDecompositionFilter(const QVector<QChar::Decomposition> &filter);
    void setMSBFilter (qint16 filter);
    int fontMSBFilter() { return m_fontMSBFilter; }

    const QFont & glyphFont() { return m_font; }

    const QVector<QChar> & supportedCharacters () { return m_supportedChars; }
    const QVector<QChar> & filteredCharacters () { return m_filteredChars; }

    const QChar & filteredCharacterAt (int pos) {
        return m_filteredChars.at(pos);
    }

    int filteredSize() { return m_filteredChars.size (); }

    const QVector<QChar::Category> & fontCategories () { return m_fontCategories; }
    QChar::Category fontCategoryAt(int pos) {return m_fontCategories.at(pos); }
    QChar::Script fontScriptAt(int pos) {return m_fontScripts.at(pos); }
    QChar::Decomposition fontDecompositionAt (int pos) {return m_fontDecompositions.at(pos);}

    const QVector<QChar::Script> & fontScripts () { return m_fontScripts; }
    const QVector<QChar::Decomposition> & fontDecompositions () { return m_fontDecompositions; }

signals:
    void glyphFontChanged (const QFont &font);
    void filteredCharactersChanged (const QVector<QChar> &characters);
    
private:
#if defined(Q_OS_WIN)
    QString getRegisterFontFilePath();
    QString getFontPathOverRegisterKey (HKEY &hKey);
#endif // Q_OS_WIN

    void saveFontManagerSettings ();
    void restoreFontManagerSettings ();

    void initDefault ();
    void initFontContext ();
    void filterCharacters ();
    void releaseFontFace ();

    QStringList customFontDirs;

    QStringList m_fontDirectory;
    QStringList m_registerFontPath;

    QString m_fontFamily;
    QString m_fontStyle;
    QString m_fontPath;

    QFont m_font;

    FT_Library m_ftLibrary;
    FT_Face m_ftFace;

    QVector<QChar> m_supportedChars;
    QVector<QChar> m_filteredChars;

    QVector<QChar::Script> m_fontScripts;
    QVector<QChar::Category> m_fontCategories;
    QVector<QChar::Decomposition> m_fontDecompositions;

    QVector<QChar::Script> m_fontScriptsFilter;
    QVector<QChar::Category> m_fontCategoriesFilter;
    QVector<QChar::Decomposition> m_fontDecompositionsFilter;

    int m_fontMSBFilter;
    int m_fontSize;
};
#endif // FONTMANAGER_H
