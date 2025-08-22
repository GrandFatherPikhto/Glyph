#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
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

    void startAsyncFontLoading();
    void resetLoadingThread();

    const FontContext & fontContext() const { return m_fontContext; }
    bool fontContextById(int id, FontContext &context);

    void setFontContext(const FontContext & value) { 
        if (m_fontContext != value)
        {
            m_fontContext = value;
            emit fontContextChanged(m_fontContext);
        }
    }

    void loadFonts(const QStringList &fontDirs);

    const QString & tableName() { return m_tableName; }

signals:
    void appendedFontContext(FontContext &context);
    void updateFontDatabase();
    void clearFontDatabase();

    void startLoading(const QStringList &dirs);

    void loadingStart (int total);
    void loadingProgress(int current, int total);
    void loadingFinished(int count);
    void loadFontContext(const FontContext &context);
    void loadingError(const QString &message);

    void changeFontContext (const FontContext &context);
    void fontContextChanged(const FontContext &context);

private:
    void setupValues ();
    void setupSignals ();
    bool createTable();

    int totalCounter(const QStringList &list);
    int processFontDir(const QString &path);

    void resetLists();
    bool writeData();
    bool appendOrUpdateFontContext(const QString & tableName, FontContext &context);
    bool isFontSupportedByFreeType(FontContext &context);

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

    FontContext m_fontContext;

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

#endif // FONTMANAGER_H
