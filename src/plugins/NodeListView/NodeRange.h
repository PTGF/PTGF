/*!
   \file NodeRange.h
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

#ifndef PLUGINS_NODELISTVIEW_NODERANGE_H
#define PLUGINS_NODELISTVIEW_NODERANGE_H

#include "NodeListViewLibrary.h"

#include <QString>
#include <QList>

#include "Node.h"

namespace Plugins {
namespace NodeListView {

class NodeRangePrivate;

class NODELISTVIEW_EXPORT NodeRange : public Node
{
    DECLARE_PRIVATE(NodeRange)

public:
    NodeRange(const QString &nodeName);
    ~NodeRange();

    virtual QString number() const;
    virtual void setNumber(const QString &number);

    QStringList expanded(const int &truncateAt = 10000) const;

    QStringList semiExpanded() const;

    bool merge(const NodeRange &other);
    bool merge(const QString &nodeName);

    quint64 count() const;

    QString toShortString() const;
};


} // namespace NodeListView
} // namespace Plugins

#endif // PLUGINS_NODELISTVIEW_NODERANGE_H
