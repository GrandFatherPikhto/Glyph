#ifndef GLYPHDRAW_GLOBAL_H
#define GLYPHDRAW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLYPHDRAW_LIBRARY)
#define GLYPHDRAW_EXPORT Q_DECL_EXPORT
#else
#define GLYPHDRAW_EXPORT Q_DECL_IMPORT
#endif

#endif // GLYPHDRAW_GLOBAL_H
