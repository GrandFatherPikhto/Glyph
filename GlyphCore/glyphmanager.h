#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QObject>
#include <QString>
#include <QSqlQuery>

#include "glyphcontext.h"
#include "profilecontext.h"

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;
class ProfileManager;
class FontManager;

class GLYPHCORE_EXPORT GlyphManager : public QObject {
    Q_OBJECT
public:
    GlyphManager(AppContext *appContext);
    ~GlyphManager();

    // bool findGlyph(const QChar &character, int profile, GlyphContext &glyph);
    GlyphContext glyph() { return m_glyph; }
    bool findGlyph(GlyphContext &context);
    bool findGlyph(int id, GlyphContext &context);
    bool appendGlyphIfNotExists(GlyphContext &context);
    bool removeGlyph(GlyphContext &context);
    bool removeGlyphById(int id);
    bool queryGlyphsByProfile(QSqlQuery &query, const ProfileContext &profile);
    // bool profileContext(const GlyphContext &glyph, ProfileContext &profile);
    // bool fontContext(const GlyphContext &glyph, FontContext &font);


signals:
    void glyphsTableChanged(const ProfileContext &profile); 
    void changeGlyph(const GlyphContext &context);
    void glyphChanged(const GlyphContext &context);
    void glyphAppended(const GlyphContext &context);
    void glyphRemoved(const GlyphContext &context);

private:
    bool queryToContext(QSqlQuery &query, GlyphContext &context);
    void setupValues ();
    void setupSignals ();
    bool createTable ();

    void saveSettings();
    void restoreSettings ();

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    ProfileManager *m_profileManager;
    FontManager *m_fontManager;

    QString m_tableName;

    GlyphContext m_glyph;
};

#endif // GLYPHMANAGER_H_
