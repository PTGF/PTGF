#ifndef COREWINDOWLIBRARY_H
#define COREWINDOWLIBRARY_H

#include <QtCore/QtGlobal>

#if defined(COREWINDOW_LIBRARY)
#  define COREWINDOW_EXPORT Q_DECL_EXPORT
#else
#  define COREWINDOW_EXPORT Q_DECL_IMPORT
#endif

#endif // COREWINDOWLIBRARY_H