#ifndef MAINWINDOWLIBRARY_H
#define MAINWINDOWLIBRARY_H

#include <QtCore/QtGlobal>

#if defined(MAINWINDOW_LIBRARY)
#  define MAINWINDOW_EXPORT Q_DECL_EXPORT
#else
#  define MAINWINDOW_EXPORT Q_DECL_IMPORT
#endif

#endif // MAINWINDOWLIBRARY_H
