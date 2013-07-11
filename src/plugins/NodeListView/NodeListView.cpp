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

#include "NodeListViewPrivate.h"

#include <QApplication>
#include <QItemSelection>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QPlainTextEdit>
#include <QLabel>
#include <QStandardItemModel>
#include <QDebug>

#include "NodeRange.h"

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
    QWidget(parent),
    d(new NodeListViewPrivate)
{
    d->q = this;

    d->m_TreeView->setHeaderHidden(true);
    d->m_TreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->m_TreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    d->m_TreeView->setModel(new QStandardItemModel(d->m_TreeView));
    connect(d->m_TreeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged()));

    d->m_txtSearch->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    d->m_txtSearch->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    d->m_txtSearch->setFixedHeight(32);
    d->m_txtSearch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    connect(d->m_txtSearch, SIGNAL(textChanged()), this, SLOT(selectNodes()));

    QFont font = d->m_lblNodeCount->font();
    font.setPointSizeF(font.pointSizeF()*0.85);
    d->m_lblNodeCount->setFont(font);
    d->m_lblNodeCount->setMargin(0);

    QVBoxLayout *baseLayout = new QVBoxLayout();
    baseLayout->addWidget(d->m_txtSearch, 0);
    baseLayout->addSpacing(10);
    baseLayout->addWidget(d->m_lblNodeCount, 0);
    baseLayout->addWidget(d->m_TreeView, 100);

    this->setLayout(baseLayout);


    // Try to get the nodelist from the Slurm environment variable
    QString slurmNodeList(getenv("SLURM_NODELIST"));
    if(!slurmNodeList.isEmpty()) {
        setNodes(slurmNodeList);
    }

}

NodeListView::~NodeListView()
{
}

QString NodeListView::nodes() const
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(d->m_TreeView->model());
    Q_ASSERT(model);
    QStandardItem *rootItem = model->invisibleRootItem();

    QList<NodeRange *> nodeList;
    for(int i = 0; i < rootItem->rowCount(); ++i) {
        QStandardItem *item = rootItem->child(i, 0);
        QString node = item->text();
        bool merged = false;

        NodeRange *newNode = new NodeRange(node);
        foreach(NodeRange *existing, nodeList) {
            if( existing->prefix().compare(newNode->prefix()) == 0 &&
                existing->suffix().compare(newNode->suffix()) == 0 &&
                existing->merge(node) ) {
                merged = true;

                delete newNode;
                newNode = NULL;

                break;
            }
        }

        if(!merged) {
            nodeList.append(newNode);
        }
    }

    QStringList nodeStringList;
    foreach(NodeRange *range, nodeList) {
        nodeStringList << range->toString();
    }

    return nodeStringList.join(",");
}

void NodeListView::setNodes(const QString &nodes)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(d->m_TreeView->model());
    Q_ASSERT(model);
    model->clear();

    QList<NodeRange*> nodeList = d->mergedNodeList(nodes);

    // Add all the individual items into the nodelist
    foreach(NodeRange *range, nodeList) {
        if(!d->m_lblNodeCount->text().isEmpty()) {
            d->m_lblNodeCount->setText(d->m_lblNodeCount->text() + ",");
        }
        d->m_lblNodeCount->setText(d->m_lblNodeCount->text() + range->toShortString());


        foreach(QString node, range->expanded()) {
            QStandardItem *item = new QStandardItem(node);
            item->setSelectable(true);
            item->setEditable(false);

            model->invisibleRootItem()->appendRow(item);
        }
    }

    qDeleteAll(nodeList);

    model->sort(0, Qt::AscendingOrder);
    d->m_TreeView->expandAll();

    d->m_lblNodeCount->setText(d->m_lblNodeCount->text() + QString("; total nodes: %1").arg(model->invisibleRootItem()->rowCount()));

    resizeSearchTextBox();

    d->m_TreeView->selectAll();
}

void NodeListView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->resizeSearchTextBox();
}


void NodeListView::selectNodes()
{
    if(d->m_SelectionChanging) {
        return;
    }

    d->m_SelectingNodes = true;

    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(d->m_TreeView->model());
    Q_ASSERT(model);
    QStandardItem *rootItem = model->invisibleRootItem();

    d->m_TreeView->clearSelection();

    QItemSelectionModel *selection = d->m_TreeView->selectionModel();
    QList<NodeRange*> nodeList = d->mergedNodeList(d->m_txtSearch->toPlainText().trimmed());
    bool error = false;
    foreach(NodeRange *range, nodeList) {
        foreach(QString node, range->expanded()) {
            bool found = false;
            for(int i = 0; i < rootItem->rowCount(); ++i) {
                QStandardItem *item = rootItem->child(i, 0);
                if(item->text().compare(node) == 0) {
                    selection->select(item->index(), QItemSelectionModel::Select | QItemSelectionModel::Rows);
                    found = true;
                    break;
                }
            }

            if(!found && !error) {
                error = true;
            }

        }
    }

    qDeleteAll(nodeList);

    if(error) {
        d->m_txtSearch->setStyleSheet("QPlainTextEdit {background-color: #FFAAAA}");
    } else {
        d->m_txtSearch->setStyleSheet("QPlainTextEdit {background-color: #FFFFFF}");
    }

    d->m_SelectingNodes = false;

    resizeSearchTextBox();
}

void NodeListView::resizeSearchTextBox()
{
    /*! \internal
        \note Unlike the documentation's description of QPlainText::document()->size(), the height of the document is the same
              as the QTextDocument::lineCount(), not the pixel height of the document.  Even scarrier, there are many projects
              blindly using this function as if it returns pixel height values, without verifying.  Oh well.  Here's my
              complete hack of a workaround.
     */

    // Let the system update the display before we exit too early
    qApp->processEvents();
    if(!d->m_txtSearch->isVisible()) {
        return;
    }
    if(d->m_PreviousLineCount == d->m_txtSearch->document()->lineCount()) {
        return;
    }

    static const int min = 20;
    static const int max = 100;
    static const int interval = qMax(sizeIncrement().height(), 1);
    int index = qBound(min, d->m_txtSearch->height(), max);
    bool direction = d->m_PreviousLineCount > d->m_txtSearch->document()->lineCount();

    while( (direction ? !d->m_txtSearch->verticalScrollBar()->isVisible() : d->m_txtSearch->verticalScrollBar()->isVisible()) &&
           index >= min &&
           index <= max) {

        d->m_txtSearch->setFixedHeight(direction ? index -= interval : index += interval);

        // Let the system update the display before we exit the loop too early
        if((direction ? d->m_txtSearch->verticalScrollBar()->isVisible() : !d->m_txtSearch->verticalScrollBar()->isVisible())) {
            qApp->processEvents();
        }

    }

    d->m_txtSearch->setFixedHeight(index+1);
    d->m_PreviousLineCount = d->m_txtSearch->document()->lineCount();
}



void NodeListView::selectionChanged()
{
    if(d->m_SelectingNodes) {
        return;
    }

    d->m_SelectionChanging = true;

    QList<NodeRange*> ranges;

    QItemSelectionModel *selection = d->m_TreeView->selectionModel();

    foreach(QModelIndex index, selection->selectedRows()) {
        QString node = index.data().toString();
        bool merged = false;
        foreach(NodeRange *range, ranges) {
            if(range->merge(node)) {
                merged = true;
                break;
            }
        }

        if(!merged) {
            ranges.append(new NodeRange(node));
        }
    }

    QStringList rangeStrings;
    foreach(NodeRange *range, ranges) {
        rangeStrings.append(range->toString());
    }

    d->m_txtSearch->setPlainText(rangeStrings.join(","));

    qDeleteAll(ranges);

    d->m_SelectionChanging = false;

}


NodeListViewPrivate::NodeListViewPrivate() :
    m_TreeView(new QTreeView),
    m_txtSearch(new QPlainTextEdit),
    m_lblNodeCount(new QLabel),
    m_PreviousLineCount(0),
    m_SelectionChanging(false),
    m_SelectingNodes(false)
{
}

NodeListViewPrivate::~NodeListViewPrivate()
{
}

QStringList NodeListViewPrivate::splitNodeList(const QString &nodes)
{
    // Break the list into NodeRanges
    QString::const_iterator i = nodes.constBegin();
    int counter = 0;
    QString nodeString;
    QStringList nodeStringList;

    while(i < nodes.constEnd()) {

        // If we're not within a node range list, and it's a comma, split here
        if(counter <= 0 && (*i) == ',') {
            if(!nodeString.isEmpty()) {
                nodeStringList << nodeString;
                nodeString.clear();
            }

            ++i;
            continue;

        // Count us as entering a node range
        } else if((*i) == '[') {
            ++counter;

        // Count us as leaving a node range
        } else if((*i) == ']') {
            --counter;

        }

        nodeString.append(*i);
        ++i;

        // If we just reached the end, add the last node
        if(i >= nodes.constEnd()) {
            if(!nodeString.isEmpty()) {
                nodeStringList << nodeString;
            }
        }
    }

    return nodeStringList;
}

QList<NodeRange*> NodeListViewPrivate::mergedNodeList(const QString &nodes)
{
    QStringList nodeStringList = splitNodeList(nodes);

    // Merge the list further, if necessary
    QList<NodeRange*> nodeList;
    foreach(QString node, nodeStringList) {
        bool merged = false;

        NodeRange tmpNode(node);
        foreach(NodeRange *existing, nodeList) {
            if( existing->prefix().compare(tmpNode.prefix()) == 0 &&
                existing->suffix().compare(tmpNode.suffix()) == 0 &&
                existing->merge(node) ) {
                merged = true;
                break;
            }
        }

        if(!merged) {
            nodeList.append(new NodeRange(node));
        }
    }
    return nodeList;
}


} // namespace NodeListView
} // namespace Plugins
