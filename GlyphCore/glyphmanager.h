#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QObject>
#include <QString>

#include "glyphcontext.h"

class AppContext;

class GlyphManager : public QObject {
    Q_OBJECT
public:
    GlyphManager(AppContext *appContext);
    ~GlyphManager();

    bool findGlyph(const QChar &character, int profile, GlyphContext &glyph);
    bool findGlyph(int id, GlyphContext &glyph);

private:
    void setupValues ();
    void setupSignals ();
    bool createTable ();

    AppContext *m_appContext;    

    QString m_tableName;
};

#endif // GLYPHMANAGER_H_
