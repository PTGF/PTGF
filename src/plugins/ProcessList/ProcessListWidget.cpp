/*!
   \file ProcessListWidget.cpp
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

#include "ProcessListWidget.h"
#include "ui_ProcessListWidget.h"

#include "ProcessListModel.h"

#include <QSortFilterProxyModel>

namespace Plugins {
namespace ProcessList {

ProcessListWidget::ProcessListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessListWidget)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel();
    proxyModel->setFilterKeyColumn(1);
    proxyModel->setFilterRole(Qt::DisplayRole);
    proxyModel->setDynamicSortFilter(true);
    ui->trvProcesses->setModel(proxyModel);
    connect(ui->txtProcessFilter, SIGNAL(textChanged(QString)), proxyModel, SLOT(setFilterRegExp(QString)));
}

ProcessListWidget::~ProcessListWidget()
{
    delete ui;
}


ProcessListModel *ProcessListWidget::model() const
{
    return qobject_cast<ProcessListModel *>(proxyModel->sourceModel());
}
void ProcessListWidget::setModel(ProcessListModel *model)
{
    ProcessListModel *oldModel = this->model();

    if(model == oldModel) {
        return;
    }

    if(oldModel) {
        disconnect(ui->btnRefresh, SIGNAL(clicked()), oldModel, SLOT(update()));
    }

    proxyModel->setSourceModel(model);

    if(model) {
        connect(ui->btnRefresh, SIGNAL(clicked()), model, SLOT(update()));
    }

}


QString ProcessListWidget::filter() const
{
    return ui->txtProcessFilter->text();
}
void ProcessListWidget::setFilter(const QString &filter)
{
    ui->txtProcessFilter->setText(filter);
}



} // namespace ProcessList
} // namespace Plugins
