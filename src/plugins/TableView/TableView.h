/*!
   \file TableView.h
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

#ifndef PLUGINS_TABLEVIEW_TABLEVIEWM_H
#define PLUGINS_TABLEVIEW_TABLEVIEWM_H

#include <QTableView>
#include <QSortFilterProxyModel>

#include <ViewManager/IViewFilterable.h>

#include "TableViewLibrary.h"
#include "Delegate.h"

namespace Plugins {
namespace TableView {

class TABLEVIEW_EXPORT TableView : public QTableView, public Core::ViewManager::IViewFilterable
{
    Q_OBJECT
    Q_INTERFACES(Core::ViewManager::IViewFilterable)

public:
    explicit TableView(QWidget *parent = 0);
    ~TableView();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QString viewFilter() const;
    void setViewFilter(const QString &regex);
    int viewFilterColumn() const;
    void setViewFilterColumn(int column = 0);

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

protected:
    Delegate m_ItemDelegate;
    QSortFilterProxyModel m_ProxyModel;

};

} // namespace TableView
} // namespace Plugins

#endif // PLUGINS_TABLEVIEW_TABLEVIEWM_H
