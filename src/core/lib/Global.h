/*!
   \file Global.h
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2013 Argo Navis Technologies, LLC

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

#ifndef PTGF_GLOBAL_H
#define PTGF_GLOBAL_H

#include <QtGlobal>

#ifndef STRINGIFY
#define STRINGIFY(X) # X
#endif

#define DECLARE_PRIVATE(Class) \
    Class##Private *d; \
    friend class Class##Private;

#define DECLARE_PUBLIC(Class) \
    Class *q; \
    friend class Class;

bool qunsetenv(const char *varName);


#endif // PTGF_GLOBAL_H
