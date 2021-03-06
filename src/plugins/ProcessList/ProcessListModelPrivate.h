/*!
   \file ProcessListPrivate.h
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

#ifndef PLUGINS_PROCESSLIST_PROCESSLISTPRIVATE_H
#define PLUGINS_PROCESSLIST_PROCESSLISTPRIVATE_H

#include "ProcessListModel.h"

namespace Plugins {
namespace ProcessList {

class PROCESSLIST_EXPORT ProcessListModelPrivate : QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(ProcessListModel)
    Q_DISABLE_COPY(ProcessListModelPrivate)

public:
    ProcessListModelPrivate();
    ~ProcessListModelPrivate();

    QByteArray getProcessList();

    bool changeBackgroundColorOnUpdate;

    QString remoteHostName;
    QString remoteShell;

    QMap<quint64, QStandardItem *> processes;
};

} // namespace ProcessList
} // namespace Plugins

#endif // PLUGINS_PROCESSLIST_PROCESSLISTPRIVATE_H
