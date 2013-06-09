/*!
   \file TableView.cpp
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

#include "TableView.h"

namespace Plugins {
namespace TableView {

/*! \class Plugins::TableView::TableView
    \brief One in the set of standard views available for the main OpenSpeeShop plugin.
    \sa QTableView QAbstractItemView
 */


/*! \fn TableView::TableView()
    \brief Constructor.
 */
TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
    m_ProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_ProxyModel.setFilterRole(Qt::EditRole);
    m_ProxyModel.setSortCaseSensitivity(Qt::CaseInsensitive);
    m_ProxyModel.setSortRole(Qt::EditRole);
    QTableView::setModel(&m_ProxyModel);

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSortingEnabled(true);

    QAbstractItemDelegate *oldDelegate = itemDelegate();
    setItemDelegate(&m_ItemDelegate);
    oldDelegate->deleteLater();
}

TableView::~TableView()
{
}


/*! \fn TableView::model()
    \brief Reimplemented in order to wrap the model in a proxy for easier filtering and sorting.
    \reimp QTableView::model()
    \sa QTableView::model TableView::setModel
 */
QAbstractItemModel *TableView::model() const
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    return proxyModel->sourceModel();
}

/*! \fn TableView::setModel()
    \brief Reimplemented in order to wrap the model in a proxy for easier filtering and sorting.
    \reimp QTableView::setModel()
    \sa QTableView::setModel TableView::model
 */
void TableView::setModel(QAbstractItemModel *model)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    proxyModel->setSourceModel(model);

    resizeColumnsToContents();
    resizeRowsToContents();
}

/*! \fn TableView::selectionChanged()
    \brief Reimplemented in order to notify the Delegate of any selection changes.
    \reimp QTableView::selectionChanged()
    \sa QTableView::selectionChanged Delegate::selected() Delegate::deselected()
 */
void TableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Delegate *delegate = qobject_cast<Delegate *>(itemDelegate());
    if(delegate) {
        foreach(QModelIndex index, selected.indexes()) {
            delegate->selected(index);
        }
        foreach(QModelIndex index, deselected.indexes()) {
            delegate->deselected(index);
        }
    }

    QTableView::selectionChanged(selected, deselected);
}


bool TableView::hasLegend()
{
    return false;
}

bool TableView::legendVisible()
{
    return false;
}

void TableView::setLegendVisible(bool visible)
{
    Q_UNUSED(visible)
}


QString TableView::viewFilter() const
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    return proxyModel->filterRegExp().pattern();
}

void TableView::setViewFilter(const QString &regex)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    selectionModel()->clear();
    proxyModel->setFilterRegExp(regex);
}

int TableView::viewFilterColumn() const
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    return proxyModel->filterKeyColumn();
}

void TableView::setViewFilterColumn(int column)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(QTableView::model());
    selectionModel()->clear();
    proxyModel->setFilterKeyColumn(column);
}

} // namespace TableView
} // namespace Plugins
