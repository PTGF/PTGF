/*!
   \file Node.cpp
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

#include "NodePrivate.h"

#include <QObject>
#include <QString>

namespace Plugins {
namespace NodeListView {

Node::Node(const QString &nodeName) :
    d(new NodePrivate)
{
    d->q = this;

    d->fromString(nodeName);
}
Node::Node() :
    d(new NodePrivate)
{
    d->q = this;
}
Node::Node(const QString &prefix, const QString &number, const QString &suffix) :
    d(new NodePrivate)
{
    d->q = this;

    d->m_Prefix = prefix;
    d->m_Value = number.toULongLong();
    d->m_Suffix = suffix;
}
Node::Node(const QString &prefix, const quint64 &value, const QString &suffix) :
    d(new NodePrivate)
{
    d->q = this;

    d->m_Prefix = prefix;
    d->m_Value = value;
    d->m_Suffix = suffix;
}

Node::~Node()
{
}


/*!
   \brief Property holds the prefix of the Node
 */
QString Node::prefix() const
{
    return d->m_Prefix;
}
/*!
   \brief Property holds the prefix of the Node
   \param prefix
 */
void Node::setPrefix(const QString &prefix)
{
    d->m_Prefix = prefix;
}


quint64 Node::value() const
{
    return d->m_Value;
}


/*!
   \brief Property holds the suffix of the Node
 */
QString Node::number() const
{
    return QString::number(d->m_Value);
}
/*!
   \brief Property holds the node ID of the Node
   \param suffix
 */
void Node::setNumber(const QString &number)
{
    bool okay;
    quint64 value = number.toLongLong(&okay);
    if(!okay) {
        throw QObject::tr("Not a number!");
    }
    d->m_Value = value;
}


/*!
   \brief Property holds the suffix of the Node
 */
QString Node::suffix() const
{
    return d->m_Suffix;
}
/*!
   \brief Property holds the suffix of the Node
   \param suffix
 */
void Node::setSuffix(const QString &suffix)
{
    d->m_Suffix = suffix;
}

/*!
   \brief Returns a range-folded string representing the entire Node
   \return
 */
QString Node::toString() const
{
    return QString("%1%2%3").arg(prefix()).arg(number()).arg(suffix());
}





NodePrivate::NodePrivate() :
    m_Value(0)
{
}

void NodePrivate::fromString(const QString &nodeName)
{
    QString prefix = "", suffix = "", number = "";
    prefix.reserve(nodeName.size());

    QString::const_iterator i = nodeName.constBegin();

    // Parse Prefix
    while(i != nodeName.constEnd() && !i->isNumber()) {
        prefix.append((*i));
        ++i;
    }

    // Parse Numeric Range(s)
    if(i != nodeName.constEnd()) {
        if(i->isNumber()) {
            do {
                number.append((*i));
                ++i;
            } while(i != nodeName.constEnd() && i->isNumber());
        }
    }

    // Parse Suffix
    while(i != nodeName.constEnd()) {
        suffix.append((*i));
        ++i;
    }

    if(!suffix.isEmpty() && suffix.endsWith(".localdomain", Qt::CaseInsensitive)) {
        suffix = suffix.remove(".localdomain", Qt::CaseInsensitive);
    }

    q->setPrefix(prefix);
    q->setNumber(number);
    q->setSuffix(suffix);
}



} // namespace NodeListView
} // namespace Plugins
