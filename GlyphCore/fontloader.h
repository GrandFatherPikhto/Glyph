#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QDir>
#include <QString>

#include <windows.h>
#include <tchar.h>

#include "fontcontext.h"

class AppContext;
class FontManager;

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT FontLoader : public QObject
{
    Q_OBJECT
public:
    explicit FontLoader(const QString &tableName, const QString &dbFile, QObject *parent = nullptr);
    ~FontLoader();

public slots:
    void loadFonts(const QStringList &fontDirs);

signals:
    void startLoading (const QStringList &list);
    void start(int total);
    void progress(int current, int total);
    void finished(int count);
    void error(const QString &message);
    void append(const FontContext &context);

private:
    int totalCounter(const QStringList &list);
    void resetLists();
    bool writeData(QSqlDatabase db);
    bool appendOrUpdateFontContext(QSqlDatabase db, const QString & tableName, FontContext &context);
    bool isFontSupportedByFreeType(FontContext &context);
    
    void setFilter(QDir &dir);
    int processFontDir(QSqlDatabase db, const QString &path);


    FontManager *m_fontManager;

    QString m_tableName;
    QString m_dbFile;
    QString m_dbConnectionName;

    int m_counter;
    int m_total;

    QMap<QString, QString> m_sysfonts;
    QStringList m_filters;

    QVariantList m_names;
    QVariantList m_families;
    QVariantList m_styles;
    QVariantList m_paths;
    QVariantList m_systems;
};

#endif // FONTLOADER_H
