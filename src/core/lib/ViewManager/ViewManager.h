/*!
   \file ViewManager.h
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

#ifndef CORE_VIEWMANAGER_VIEWMANAGER_H
#define CORE_VIEWMANAGER_VIEWMANAGER_H

#include <QObject>
class QAbstractItemView;
class QAbstractItemModel;

#include "ViewManagerLibrary.h"

namespace Core {
namespace ViewManager {

class ViewManagerPrivate;

class VIEWMANAGER_EXPORT ViewManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewManager)
    DECLARE_PRIVATE(ViewManager)

public:
    static ViewManager &instance();
    ~ViewManager();

    bool initialize();
    void shutdown();

    QStringList viewNames(QAbstractItemModel *model = NULL);
    QAbstractItemView *viewWidget(QString name, QAbstractItemModel *model);

private:
    explicit ViewManager();

};

} // namespace ViewManager
} // namespace Core

#endif // CORE_VIEWMANAGER_VIEWMANAGER_H
