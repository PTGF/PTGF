/*!
   \file NodePrivate.h
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

#ifndef PLUGINS_NODELISTVIEW_NODEPRIVATE_H
#define PLUGINS_NODELISTVIEW_NODEPRIVATE_H

#include "Node.h"

#include <QString>

namespace Plugins {
namespace NodeListView {

class NodePrivate
{
    DECLARE_PUBLIC(Node)

public:
    NodePrivate();
    ~NodePrivate() {}

    void fromString(const QString &nodeName);

private:
    QString m_Prefix;
    quint64 m_Value;
    QString m_Suffix;
};


} // namespace NodeListView
} // namespace Plugins




#endif // PLUGINS_NODELISTVIEW_NODEPRIVATE_H
