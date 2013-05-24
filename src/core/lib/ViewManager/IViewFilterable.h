/*!
   \file IViewFilterable.h
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


#ifndef CORE_VIEWMANAGER_IVIEWFILTERABLE_H
#define CORE_VIEWMANAGER_IVIEWFILTERABLE_H

#include <QObject>

#include "ViewManagerLibrary.h"

namespace Core {
namespace ViewManager {

class VIEWMANAGER_EXPORT IViewFilterable
{
public:
    virtual QString viewFilter() const = 0;
    virtual void setViewFilter(const QString &regex) = 0;
    virtual int viewFilterColumn() const = 0;
    virtual void setViewFilterColumn(int column = 0) = 0;
};

} // namespace ViewManager
} // namespace Core

Q_DECLARE_INTERFACE(Core::ViewManager::IViewFilterable, "org.krellinst.ptgf.IViewFilterable")

#endif // CORE_VIEWMANAGER_IVIEWFILTERABLE_H
