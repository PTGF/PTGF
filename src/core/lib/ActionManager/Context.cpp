/*!
   \file Context.cpp
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

#include "ContextPrivate.h"

namespace Core {
namespace ActionManager {

/*!
   \class Context
   \todo Better documentation
   \brief Core::ActionManager::Context
 */

/*!
   \brief Context::Context
   \todo Better documentation
   \param parent
 */
Context::Context(Context *parent) :
    d(new ContextPrivate)
{
    d->q = this;
    setParent(parent);
}

/*! \internal
 */
Context::~Context()
{
    //TODO: Remove self from ActionManager
}

/*!
   \brief parent
   \todo Better documentation
   \return
 */
Context *Context::parentContext() const
{
    if(QObject *parent = this->parent()) {
        return qobject_cast<Context *>(parent);
    }
    return NULL;
}

/*!
   \brief Sets the parent of the Context to parent, and resets all flags.
   If the new parent Context is the same as the old parent, this function does nothing.
   \todo Better documentation
   \param parent
 */
void Context::setParent(Context *parent)
{
    if(parent == parentContext()) {
        return;
    }

    disconnect(d.data(), SLOT(parentChanged()));

    QObject::setParent(parent);

    if(parent) {
        connect(parent, SIGNAL(changed()), d.data(), SLOT(parentChanged()));
    }

    emit changed();
}


/*!
   \brief Context::contexts
   \todo Better documentation
   \return
 */
QList<Context *> Context::contexts() const
{
    return findChildren<Context *>();
}

/*!
   \brief Context::isEnabled
   \todo Better documentation
   \return
 */
bool Context::isEnabled()
{
    /* We have to be visible
       AND
       All parents have to also be enabled
       AND
       We have to be enabled
     */
    return (isVisible() && d->parentEnabled() && d->m_Enabled);
}

/*!
   \brief Context::setEnabled
   \todo Better documentation
   \param enable
 */
void Context::setEnabled(bool enable)
{
    d->m_Enabled = enable;
    emit changed();
    emit enableChanged(d->m_Enabled);
}

/*!
   \brief Context::isVisible
   \todo Better documentation
   \return
 */
bool Context::isVisible()
{
    /* All parents have to also be visible
       AND
       We have to be visible
     */
    return (d->parentVisible() && d->m_Visible);
}
/*!
   \brief Context::setVisible
   \todo Better documentation
   \param visible
 */
void Context::setVisible(bool visible)
{
    d->m_Visible = visible;
    emit changed();
    emit visibleChanged(d->m_Visible);
}

/*!
   \brief Context::enable
   \todo Better documentation
 */

/*!
   \brief Context::disable
   \todo Better documentation
 */

/*!
   \brief Context::show
   \todo Better documentation
 */

/*!
   \brief Context::hide
   \todo Better documentation
 */

/***** PRIVATE IMPLEMENTATION *****/

/*!
   \internal
   \brief ContextPrivate::ContextPrivate
 */
ContextPrivate::ContextPrivate() :
    q(NULL),
    m_Enabled(true),
    m_Visible(true)
{
}

/*!
   \internal
   \brief ContextPrivate::parentEnabled
   \return
 */
bool ContextPrivate::parentEnabled()
{
    if(Context *parent = q->parentContext()) {
        return parent->isEnabled();
    }
    return true;
}

/*!
   \internal
   \brief ContextPrivate::parentVisible
   \return
 */
bool ContextPrivate::parentVisible()
{
    if(Context *parent = q->parentContext()) {
        return parent->isVisible();
    }
    return true;
}


void ContextPrivate::parentChanged()
{
    emit q->changed();
}


} // namespace ActionManager
} // namespace Core

