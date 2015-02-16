/*!
   \file NodeListViewLibrary.h
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

#ifndef PLUGINS_NODELISTVIEW_NODELISTVIEWLIBRARY_H
#define PLUGINS_NODELISTVIEW_NODELISTVIEWLIBRARY_H

#include <Global.h>

#if defined(NODELISTVIEW_LIBRARY)
#  define NODELISTVIEW_EXPORT Q_DECL_EXPORT
#else
#  define NODELISTVIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINS_NODELISTVIEW_NODELISTVIEWLIBRARY_H
