#ifndef NOTIFICATIONMANAGERLIBRARY_H
#define NOTIFICATIONMANAGERLIBRARY_H

#include <QtCore/QtGlobal>

#if defined(NOTIFICATIONMANAGER_LIBRARY)
#  define NOTIFICATIONMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define NOTIFICATIONMANAGER_EXPORT Q_DECL_IMPORT
#endif

#endif // NOTIFICATIONMANAGERLIBRARY_H