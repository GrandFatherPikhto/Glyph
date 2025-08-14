#ifndef IMAGEMANAGER_H_
#define IMAGERMANAGER_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;
class GlyphManager;
class ProfileManager;

class GLYPHCORE_EXPORT ImageManager : public QObject {
    Q_OBJECT
public:

    ImageManager (AppContext *appContext);
    ~ImageManager ();

private:
    void initValues();
    void createTable ();

    AppContext *m_appContext;
    AppSettings * m_appSettings;
    GlyphManager *m_glyphManager;
    ProfileManager *m_profileManager;
};

#endif // IMAGEMANAGER_H_