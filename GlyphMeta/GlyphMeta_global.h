#ifndef GLYPHMETA_GLOBAL_H
#define GLYPHMETA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLYPHMETA_LIBRARY)
#define GLYPHMETA_EXPORT Q_DECL_EXPORT
#else
#define GLYPHMETA_EXPORT Q_DECL_IMPORT
#endif

#endif // GLYPHMETA_GLOBAL_H
