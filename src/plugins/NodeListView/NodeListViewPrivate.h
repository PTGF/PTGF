/*!
   \file NodeListViewPrivate.h
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

#ifndef PLUGINS_NODELISTVIEW_NODELISTVIEWPRIVATE_H
#define PLUGINS_NODELISTVIEW_NODELISTVIEWPRIVATE_H

#include "NodeListView.h"

#include "Node.h"

class QTreeView;
class QPlainTextEdit;
class QLabel;
class QStandardItem;

namespace Plugins {
namespace NodeListView {

class NODELISTVIEW_EXPORT NodeListViewPrivate : QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(NodeListView)
    Q_DISABLE_COPY(NodeListViewPrivate)

public:
    NodeListViewPrivate();
    ~NodeListViewPrivate();

    QStringList splitNodeList(const QString &nodes, bool *okay = 0);
    QList<NodeRange*> mergedNodeList(const QString &nodeList, bool *okay = 0);

protected slots:
    void resize();
    void selectNodes();
    void resizeSearchTextBox();
    void selectionChanged();

private:
    QTreeView *m_TreeView;
    QPlainTextEdit *m_txtSearch;
    QLabel *m_lblNodeCount;

    int m_PreviousLineCount;

    bool m_SelectionChanging;
    bool m_SelectingNodes;

    bool m_IsValid;
};

} // namespace NodeListView
} // namespace Plugins

#endif // PLUGINS_NODELISTVIEW_NODELISTVIEWPRIVATE_H
