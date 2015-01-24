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

#include <ViewManager/IView.h>
#include <ViewManager/IViewFilterable.h>

#include "TableViewLibrary.h"
#include "Delegate.h"

namespace Plugins {
namespace TableView {

class TABLEVIEW_EXPORT TableView : public QTableView, public Core::ViewManager::IViewFilterable, public Core::ViewManager::IView
{
    Q_OBJECT
    Q_INTERFACES(Core::ViewManager::IView)
    Q_INTERFACES(Core::ViewManager::IViewFilterable)

public:
    explicit TableView(QWidget *parent = 0);
    ~TableView();

    virtual QAbstractItemModel *model() const;
    virtual void setModel(QAbstractItemModel *model);

    virtual void setItemDelegate(QAbstractItemDelegate *delegate);

    virtual bool hasLegend();
    virtual bool legendVisible();
    virtual void setLegendVisible(bool visible);

    virtual QString viewFilter() const;
    virtual void setViewFilter(const QString &regex);
    virtual int viewFilterColumn() const;
    virtual void setViewFilterColumn(int column = 0);

protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void delegateSizeHintChanged(const QModelIndex &index);

protected:
    QSortFilterProxyModel m_ProxyModel;

};

} // namespace TableView
} // namespace Plugins

#endif // PLUGINS_TABLEVIEW_TABLEVIEWM_H
