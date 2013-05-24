/*!
   \file AbstractView.h
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

#ifndef CORE_VIEWMANAGER_ABSTRACTVIEW_H
#define CORE_VIEWMANAGER_ABSTRACTVIEW_H

#include <QAbstractItemView>

#include "ViewManagerLibrary.h"

namespace Core {
namespace ViewManager {

class VIEWMANAGER_EXPORT AbstractView : public QAbstractItemView
{
    Q_OBJECT

public:
    explicit AbstractView(QWidget *parent = 0);

    virtual bool hasLegend();
    virtual bool legendVisible();
    virtual void setLegendVisible(bool visible);


//    /* BEGIN QAbstractItemView interface */
//public:
//    virtual void setModel(QAbstractItemModel *model) = 0;
//    virtual QRect visualRect(const QModelIndex &index) const = 0;
//    virtual void scrollTo(const QModelIndex &index,ScrollHint hint=EnsureVisible) = 0;
//    virtual QModelIndex indexAt(const QPoint &point) const = 0;
//protected:
//    virtual QModelIndex moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers) = 0;
//    virtual int horizontalOffset() const = 0;
//    virtual int verticalOffset() const = 0;
//    virtual bool isIndexHidden(const QModelIndex &index) const = 0;
//    virtual void setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command) = 0;
//    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const = 0;
//    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) = 0;
//    /* END QAbstractItemView interface */

};

} // namespace ViewManager
} // namespace Core

#endif // CORE_VIEWMANAGER_ABSTRACTVIEW_H
