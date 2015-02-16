/*!
   \file ViewManagerPrivate.h
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

#ifndef CORE_VIEWMANAGER_VIEWMANAGERPRIVATE_H
#define CORE_VIEWMANAGER_VIEWMANAGERPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include "ViewManager.h"

#include <QObject>
#include <QMap>


namespace Core {
namespace ViewManager {

class IViewFactory;

class ViewManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewManagerPrivate)
    DECLARE_PUBLIC(ViewManager)

public:
    explicit ViewManagerPrivate();

protected:
    void registerViewFactory(IViewFactory *viewFactory);
    void deregisterViewFactory(IViewFactory *viewFactory);

protected slots:
    void pluginObjectRegistered(QObject *object);
    void pluginObjectDeregistered(QObject *object);

private:
    bool m_Initialized;
    QMap<QString, IViewFactory *> m_viewFactories;

};

} // namespace ViewManager
} // namespace Core

#endif // CORE_VIEWMANAGER_VIEWMANAGERPRIVATE_H
