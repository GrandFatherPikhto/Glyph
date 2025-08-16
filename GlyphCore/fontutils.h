#ifndef FONTUTILS_H
#define FONTUTILS_H

#include <QMap>
#include <QString>
#include <QChar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFontDatabase>

#include <windows.h>
#include <tchar.h>
#include <Shlobj.h>

#include <iostream>

#include "fontcontext.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

#include "GlyphCore_global.h"

bool GLYPHCORE_EXPORT getWinSystemFonts(QMap<QString, QString> &fonts, const HKEY &rootKey = HKEY_LOCAL_MACHINE);
bool GLYPHCORE_EXPORT getRegisterKeyValue(const HKEY &hKey, DWORD index, QString &name, QString &data);
bool GLYPHCORE_EXPORT appendOrUpdateFontContext(QSqlDatabase db, const QString & tableName, FontContext &context);
bool GLYPHCORE_EXPORT isFontSupportedByFreeType(FontContext &context);
QString GLYPHCORE_EXPORT winDefaultFontPath();
QFont GLYPHCORE_EXPORT fontByPath(const QString &fontPath);

#endif // FONTUTILS_H
