#ifndef GLYPHUI_GLOBAL_H
#define GLYPHUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLYPHUI_LIBRARY)
#define GLYPHUI_EXPORT Q_DECL_EXPORT
#else
#define GLYPHUI_EXPORT Q_DECL_IMPORT
#endif

#endif // GLYPHUI_GLOBAL_H
