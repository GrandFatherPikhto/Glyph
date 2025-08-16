#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
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
    void progress(int current, int total);
    void finished(int count);
    void error(const QString &message);
    void addedFontContext(const FontContext &context);

private:
    int processFontDir(QSqlDatabase db, const QString &path);

    FontManager *m_fontManager;

    QString m_tableName;
    QString m_dbFile;
    QString m_dbConnectionName;

    QMap<QString, QString> m_sysfonts;
};

#endif // FONTLOADER_H
