#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QObject>
#include <QString>
#include <QSqlQuery>

#include "glyphcontext.h"

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;

class GLYPHCORE_EXPORT GlyphManager : public QObject {
    Q_OBJECT
public:
    GlyphManager(AppContext *appContext);
    ~GlyphManager();

    // bool findGlyph(const QChar &character, int profile, GlyphContext &glyph);
    bool findGlyph(GlyphContext &context);
    bool findGlyph(int id, GlyphContext &context);
    bool appendGlyphIfNotExists(GlyphContext &context);

private:
    bool queryToContext(QSqlQuery &query, GlyphContext &context);
    void setupValues ();
    void setupSignals ();
    bool createTable ();

    AppContext *m_appContext;
    AppSettings *m_appSettings;

    QString m_tableName;
};

#endif // GLYPHMANAGER_H_
