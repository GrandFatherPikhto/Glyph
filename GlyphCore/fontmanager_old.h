#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>
#include <QFontDatabase>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QtGlobal>
#include <QSettings>
#include <QVector>

#include <windows.h>
#include <tchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

#include "GlyphCore_global.h"

#include "fontcontext.h"

class AppContext;

class GLYPHCORE_EXPORT FontManager : public QObject
{
    Q_OBJECT
public:
    explicit FontManager(AppContext *appContext);
    ~FontManager();

    bool loadFontContext (FontContext &context);

    const FontContext &fontContext() const { return m_fontContext; }

signals:


private:
    void setupValues ();
    void setupSignals ();
    
    void initContext ();

#if defined(Q_OS_WIN)
    bool getRegisterFontFilePath();
    QString getFontPathOverRegisterKey(HKEY &hKey);
#endif

    // QFont m_font;
    // QString m_fontPath;
    // QString m_fontFamily;
    FontContext m_fontContext;
    QStringList m_fontDirectories;
    QStringList m_fontRegisterPaths;

    AppContext *m_appContext;
};

#endif // FONTMANAGER_H
