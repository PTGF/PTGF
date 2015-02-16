/*!
   \file Delegate.h
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

#ifndef PLUGINS_TABLEVIEW_DELEGATE_H
#define PLUGINS_TABLEVIEW_DELEGATE_H

#include <QStyledItemDelegate>
#include <QSet>

#include "TableViewLibrary.h"

namespace Plugins {
namespace TableView {

class TABLEVIEW_EXPORT Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

public slots:
    virtual void selected(const QModelIndex &index);
    virtual void deselected(const QModelIndex &index);

protected:
    QSet<QModelIndex> m_SelectedRows;

};

} // namespace TableView
} // namespace Plugins

#endif // PLUGINS_TABLEVIEW_DELEGATE_H
