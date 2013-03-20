#ifndef PTGF_GLOBAL_H
#define PTGF_GLOBAL_H

#include <qglobal.h>
#include <QByteArray>

#include <stdlib.h>

bool qunsetenv(const char *varName) {
#if defined(_POSIX_VERSION) && (_POSIX_VERSION-0) >= 200112L
    // POSIX.1-2001 has unsetenv
    return unsetenv(varName) == 0;
#else
    return qputenv(varName, QByteArray());
#endif
}


#endif // PTGF_GLOBAL_H
