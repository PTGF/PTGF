/*!
   \file NodeListView.cpp
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

#include "NodeListView.h"

namespace Plugins {
namespace NodeListView {


/*! \class Plugins::NodeListView::NodeListView
    \brief

    The node list widget will be a common element to all tools allowing the user to manually select from a list of nodes
    using the mouse, or using a textual representation of a node list, called "node set" in the rest of this document.
    This node set will automatically be grouped by job allocation, with job allocations for the current user being
    pre-selected. Further, a user could manually select nodes 1,2,3,5, and 10 using the mouse, or could type "1-3,5,10"
    into a text box. This allows both novice and expert users to quickly select node sets for use with a tool.
 */


NodeListView::NodeListView(QWidget *parent) :
    QTreeView(parent)
{
}

} // namespace NodeListView
} // namespace Plugins
