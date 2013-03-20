#ifndef WINDOWMANGERLIBRARY_H
#define WINDOWMANGERLIBRARY_H

#include <QtCore/QtGlobal>

#if defined(WINDOWMANAGER_LIBRARY)
#  define WINDOWMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define WINDOWMANAGER_EXPORT Q_DECL_IMPORT
#endif

#endif // WINDOWMANGERLIBRARY_H