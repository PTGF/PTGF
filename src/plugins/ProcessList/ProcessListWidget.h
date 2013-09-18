/*!
   \file ProcessListWidget.h
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

#ifndef PLUGINS_PROCESSLIST_PROCESSLISTWIDGET_H
#define PLUGINS_PROCESSLIST_PROCESSLISTWIDGET_H

#include <QWidget>

class QSortFilterProxyModel;

namespace Plugins {
namespace ProcessList {

namespace Ui { class ProcessListWidget; }

class ProcessListModel;

class ProcessListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessListWidget(QWidget *parent = 0);
    ~ProcessListWidget();

    virtual ProcessListModel *model() const;
    virtual void setModel(ProcessListModel *model);

    QString filter() const;
    void setFilter(const QString &filter);

private:
    Ui::ProcessListWidget *ui;
    QSortFilterProxyModel *proxyModel;
};

} // namespace ProcessList
} // namespace Plugins

#endif // PLUGINS_PROCESSLIST_PROCESSLISTWIDGET_H
