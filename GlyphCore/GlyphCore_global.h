#ifndef GLYPHCORE_GLOBAL_H
#define GLYPHCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLYPHCORE_LIBRARY)
#define GLYPHCORE_EXPORT Q_DECL_EXPORT
#else
#define GLYPHCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // GLYPHCORE_GLOBAL_H
