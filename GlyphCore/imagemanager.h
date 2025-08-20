#ifndef IMAGEMANAGER_H_
#define IMAGERMANAGER_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSharedPointer>
#include <QImage>
#include <QSqlQuery>

#include "GlyphCore_global.h"

#include "imagecontext.h"
#include "fontcontext.h"
#include "iglyphrenderer.h"
#include "drawcontext.h"

class AppContext;
class AppSettings;
class GlyphManager;
class ProfileManager;

class GLYPHCORE_EXPORT ImageManager : public QObject {
    Q_OBJECT
public:

    ImageManager (AppContext *appContext);
    ~ImageManager ();

    bool getGlyphImage(const FontContext &font, QSharedPointer<ImageContext> &image);

    bool saveDrawImage(const QSharedPointer<DrawContext> &draw);
    bool findDrawImage(int glyphId, QSharedPointer<DrawContext> &draw);
    bool loadOrCreateDrawImage(int glyphId, QSharedPointer<DrawContext> &draw);
    
private:
    void initValues();
    void setupSignals ();
    bool createTable ();
    
    bool loadFromQuery(QSqlQuery query, QSharedPointer<DrawContext> &draw);

    AppContext *m_appContext;
    AppSettings *m_appSettings;
    GlyphManager *m_glyphManager;
    ProfileManager *m_profileManager;

    std::unique_ptr<IGlyphRenderer> m_glyphRenderer;

    QString m_tableName;
};

#endif // IMAGEMANAGER_H_
