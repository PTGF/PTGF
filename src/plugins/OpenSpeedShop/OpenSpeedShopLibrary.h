#ifndef OPENSPEEDSHOPLIBRARY_H
#define OPENSPEEDSHOPLIBRARY_H

#include <QtCore/QtGlobal>

#if defined(OPENSPEEDSHOP_LIBRARY)
#  define OPENSPEEDSHOP_EXPORT Q_DECL_EXPORT
#else
#  define OPENSPEEDSHOP_EXPORT Q_DECL_IMPORT
#endif

#endif // OPENSPEEDSHOPLIBRARY_H
