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
#include <QDir>
#include <QThread>

#include <windows.h>
#include <tchar.h>

#include "fontcontext.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class AppContext;
class FontLoader;
class DbManager; 

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT FontManager : public QObject
{
    Q_OBJECT
public:
    explicit FontManager(AppContext *appContext);
    ~FontManager();

    QString winDefaultFontPath ();
    QStringList fontFiles(const QString &path);
    bool isSupportedByFreeType(FontContext &context);
    QFont fontByPath(const QString &path);

    // int updateFontList(const QString &path);
    void startAsyncFontLoading();
    bool appendOrUpdateFontContext(FontContext &context);

    bool createTable();

signals:
    void appendedFontContext(FontContext &context);
    void updateFontDatabase();
    void clearFontDatabase();

    void startLoading (const QStringList &fontDirs);
    void loadingProgress(int current, int total);
    void loadingFinished(int count);
    void loadingError(const QString &message);
    void addedFontContext(const FontContext &context);

    void addFontContext(const FontContext &context);

private:
    bool setDefaultFontPath();
    void saveFontManagerSettings();
    void restoreFontManagerSettings();

    QStringList m_fontDirs;
    AppContext *m_appContext;
    DbManager *m_dbManager;

    QStringList m_fontFiles;

    QString m_tableName;

    QThread m_workerThread;
    FontLoader *m_fontLoader;
};

#endif // FONTMANAGER_H
