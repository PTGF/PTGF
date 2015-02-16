/*!
   \file ProcessList.h
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

#ifndef PLUGINS_PROCESSLIST_PROCESSLIST_H
#define PLUGINS_PROCESSLIST_PROCESSLIST_H

#include "ProcessListLibrary.h"

#include <QStandardItemModel>

namespace Plugins {
namespace ProcessList {

class ProcessListModelPrivate;


class PROCESSLIST_EXPORT ProcessListModel : public QStandardItemModel
{
    Q_OBJECT
    DECLARE_PRIVATE(ProcessListModel)
    Q_DISABLE_COPY(ProcessListModel)

public:
    explicit ProcessListModel(QObject *parent = 0);
    ~ProcessListModel();

    QString remoteHost() const;
    void setRemoteHost(const QString &hostName);

    QString remoteShell() const;
    void setRemoteShell(const QString &shell);

    bool changeBackgroundColorOnUpdate() const;
    void setChangeBackgroundColorOnUpdate(const bool &change);

public slots:
    void update();

};


} // namespace ProcessList
} // namespace Plugins

#endif // PLUGINS_PROCESSLIST_PROCESSLIST_H
