#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <QObject>
#include <QFont>
#include <QString>
#include <QChar>
#include <QSqlQuery>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

#include <freetype/fterrors.h>

#include "charmapkey.h"
#include "fontcontext.h"

#include "GlyphCore_global.h"

class AppContext;
class FontManager;
class SqlFilter;
class DbCore;
class UnicodeMetadataSelectionModel;
class AppSettings;
class ProfileManager;

class GLYPHCORE_EXPORT CharmapManager : public QObject
{
    Q_OBJECT
public:
    explicit CharmapManager(AppContext *appContext);
    ~CharmapManager();

    bool loadFont(const FontContext &context);

    const QString tableName() { return m_tableName; }

    SqlFilter *filter() { return m_filter; }

    // void setTextFilter(const QString &chars);
    // void setMSBFilter(int value);

//     QString updateQuery();
    bool execQuery(QSqlQuery &query, const QString &endSql="ORDER BY c.unicode ASC");
    bool isFilter();

signals:
    void charmapUpdated(const FontContext &context);

private:
    void setupValues ();
    void setupSignals ();
    
    void initFtLibrary ();
    void releaseFtLibrary ();
    void releaseFontFace ();

    bool loadFontFace();
    bool loadCharacterTable();
    bool createCharacterTable ();

    QStringList find(const CharmapKey &key);
    bool remove(const CharmapKey &key);



    FT_Library m_ftLibrary;
    FT_Face m_ftFace;

    FontContext m_fontContext;

    AppContext *m_appContext;
    FontManager *m_fontManager;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;

    SqlFilter *m_filter;

    QString m_tableName;

    QString m_sortCondition;

    QString m_charsFilter;

    int m_msb;
};

#endif // CHARACTERMANAGER_H
