#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "GlyphCore_global.h"

class DbCore;
class AppSettings;
class FontManager;
class CharmapManager;
class UnicodeMetadata;
class ProfileManager;
class GlyphManager;
class ImageManager;

class GLYPHCORE_EXPORT AppContext : public QObject
{
    Q_OBJECT
public:    
    AppContext(QObject *parent=nullptr);
    ~AppContext();

    DbCore * dbCore() { return m_dbCore; }
    FontManager *fontManager() { return m_fontManager; }
    CharmapManager *characterManager() { return m_charmapManager; }
    UnicodeMetadata *unicodeMetadata () { return m_unicodeMetadata; }
    ProfileManager *profileManager() { return m_profileManager; }
    AppSettings *appSettings() { return m_appSettings; }
    GlyphManager *glyphManager() { return m_glyphManager; }
    ImageManager *ImageManager() { return m_imageManager; }

signals:
    void valuesInited ();    

private:
    void setupVariables();

    DbCore *m_dbCore;
    AppSettings *m_appSettings;
    FontManager *m_fontManager;
    CharmapManager *m_charmapManager;
    UnicodeMetadata *m_unicodeMetadata;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    ImageManager *m_imageManager;
};

#endif // APPCONTEXT_H
