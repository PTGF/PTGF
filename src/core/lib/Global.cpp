/*!
   \file Global.cpp
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2015 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */

#include "Global.h"

#if defined(_POSIX_VERSION) && (_POSIX_VERSION-0) >= 200112L
// POSIX.1-2001 has unsetenv
#include <stdlib.h>
bool qunsetenv(const char *varName) {
    return unsetenv(varName) == 0;
}
#else
#include <QByteArray>
bool qunsetenv(const char *varName) {
    return qputenv(varName, QByteArray());
}
#endif
