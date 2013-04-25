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
class ActionManager;

class ACTIONMANAGER_EXPORT Context : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enableChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)

    Q_DISABLE_COPY(Context)
    DECLARE_PRIVATE(Context)

public:
    ~Context();

    Context *parentContext() const;
    void setParent(Context *parent);

    QList<Context *> contexts() const;

    bool isEnabled();

    bool isVisible();

signals:
    void changed();
    void enableChanged(bool enabled);
    void visibleChanged(bool visible);

public slots:
    void setEnabled(bool enable);
    inline void enable() { setEnabled(true); }
    inline void disable() { setEnabled(false); }

    void setVisible(bool visible);
    inline void show() { setVisible(true); }
    inline void hide() { setVisible(false); }

private:
    explicit Context(Context *parent = 0);

    friend class ActionManager;
};

} // namespace ActionManager
} // namespace Core

#endif // CORE_ACTIONMANAGER_CONTEXT_H
