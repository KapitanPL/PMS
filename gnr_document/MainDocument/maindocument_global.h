#ifndef MAINDOCUMENT_GLOBAL_H
#define MAINDOCUMENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAINDOCUMENT_LIBRARY)
#  define DLLCAL Q_DECL_EXPORT
#else
#  define DLLCAL Q_DECL_IMPORT
#endif

#endif // MAINDOCUMENT_GLOBAL_H
