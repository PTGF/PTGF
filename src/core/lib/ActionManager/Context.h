/*!
   \file Context.h
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

#ifndef CORE_ACTIONMANAGER_CONTEXT_H
#define CORE_ACTIONMANAGER_CONTEXT_H

#include <QObject>
#include "ActionManagerLibrary.h"

namespace Core {
namespace ActionManager {

class ContextPrivate;

class ACTIONMANAGER_EXPORT Context : public QObject
{
    Q_OBJECT
    DECLARE_PRIVATE(Context)

public:
    explicit Context(QObject *parent = 0);
    ~Context();

    QList<Context *> contexts() const;

    bool isEnabled();
    void setEnabled(bool enable);

signals:
    void enabled();
    void disabled();

public slots:
    void enable();
    void disable();

};

} // namespace ActionManager
} // namespace Core

#endif // CORE_ACTIONMANAGER_CONTEXT_H
