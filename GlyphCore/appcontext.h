#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "GlyphCore_global.h"

class AppSettings;
class FontManager;
class CharmapManager;
class UnicodeMetadata;
class ProfileManager;
class GlyphManager;
class ImageManager;
class DbManager;
class GridManager;

class GLYPHCORE_EXPORT AppContext : public QObject
{
    Q_OBJECT
public:    
    AppContext(QObject *parent=nullptr);
    ~AppContext();

    FontManager * fontManager();
    CharmapManager * charmapManager();
    UnicodeMetadata * unicodeMetadata();
    ProfileManager * profileManager();
    AppSettings * appSettings();
    GlyphManager * glyphManager();
    ImageManager * imageManager();
    DbManager * dbManager();
    GridManager *gridManager();

    const QString & appUserDir() const { return m_appUserDir; }

signals:

private:
    void setupVariables();
    void initAppUserDir();

    AppSettings *m_appSettings;
    FontManager *m_fontManager;
    CharmapManager *m_charmapManager;
    UnicodeMetadata *m_unicodeMetadata;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    ImageManager *m_imageManager;
    DbManager *m_dbManager;
    GridManager *m_gridManager;

    QString m_appUserDir;
};

#endif // APPCONTEXT_H
