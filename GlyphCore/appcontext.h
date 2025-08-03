#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QImage>
#include <QSettings>
#include <QColor>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QMargins>
#include <QPainter>

class GlyphManager;
class FontManager;
class GlyphImageManager;
class UnicodeMetadata;
class AppSettings;
class GlyphFilter;
class DimensionManager;
class AppData;
class DbCore;
class AppProject;

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT AppContext : public QObject
{
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext();

    void setupSignals();

    GlyphManager * glyphManager() { return m_glyphManager; }
    FontManager * fontManager() { return m_fontManager; }
    AppSettings * appSettings() { return m_appSettings; }
    UnicodeMetadata * unicodeMetadata () { return m_unicodeMetadata; }
    DimensionManager * dimensionManager () { return m_dimensionManager; }
    GlyphImageManager * glyphImageManager () { return m_imageManager; }
    GlyphFilter * glyphFilter () { return m_glyphFilter; }
    AppData * appData() { return m_appData; }
    DbCore * dbCore() { return m_dbCore; }
    AppProject * appProject() { return m_appProject; }

    // Методы
    void saveAppContext ();
    void restoreAppContext ();

signals:

    void leftGridCellsChanged   (int value);
    void bottomGridCellsChanged (int value);

    void fontMSBChanged(int value);
    void fontChanged (const QFont &newFont);
    void fontCategoriesChanged (const QVector<QChar::Category> &value);
    void fontScriptsChanged (const QVector<QChar::Script> &value);
    void fontDecompositionsChanged (const QVector<QChar::Decomposition> &value);

    void pasteTemplateToDrawLayer();
    void clearDrawLayer ();
    
public slots:

private:
    void initValues ();
    
    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    DimensionManager *m_dimensionManager;
    GlyphImageManager *m_imageManager;
    UnicodeMetadata *m_unicodeMetadata;
    AppSettings *m_appSettings;
    GlyphFilter *m_glyphFilter;
    DbCore *m_dbCore;
    AppData *m_appData;
    AppProject *m_appProject;
};

Q_DECLARE_METATYPE(AppContext)

#endif // APPCONTEXT_H
