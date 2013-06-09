/*!
   \file NodeListView.h
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

#ifndef PLUGINS_NODELISTVIEW_NODELISTVIEW_H
#define PLUGINS_NODELISTVIEW_NODELISTVIEW_H

#include <QTreeView>

#include "NodeListViewLibrary.h"

namespace Plugins {
namespace NodeListView {

class NODELISTVIEW_EXPORT NodeListView : public QTreeView
{
    Q_OBJECT
    Q_DISABLE_COPY(NodeListView)

public:
    explicit NodeListView(QWidget *parent = 0);

signals:

public slots:

};

} // namespace NodeListView
} // namespace Plugins

#endif // PLUGINS_NODELISTVIEW_NODELISTVIEW_H
