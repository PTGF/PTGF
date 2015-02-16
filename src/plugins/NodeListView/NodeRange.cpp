/*!
   \file NodeRange.cpp
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

#include "NodeRangePrivate.h"

#include <QString>
#include <QStringList>
#include <QDebug>

namespace Plugins {
namespace NodeListView {

/*!
   \brief Creates a NodeRange object from the given nodeName string
   \param nodeName
 */
NodeRange::NodeRange(const QString &nodeName) :
    Node(),
    d(new NodeRangePrivate)
{
    d->q = this;

    merge(nodeName);
}

NodeRange::~NodeRange()
{
}



bool NodeRange::contains(const QString &nodeName)
{
    if(!d->m_Initialized) {
        return false;
    }


    QString prefix = "", suffix = "", number = "";
    prefix.reserve(nodeName.size());

    QString::const_iterator i = nodeName.constBegin();

    // Parse Prefix
    while(i < nodeName.constEnd() && !i->isNumber()) {
        prefix.append((*i));
        ++i;
    }

    // Verify we have a valid prefix
    if(this->prefix() != prefix) {
        return false;
    }


    // Parse the number
    if(i < nodeName.constEnd()) {
        if(i->isNumber()) {
            do {
                number.append((*i));
                ++i;
            } while(i < nodeName.constEnd() && i->isNumber());
        } else {
            qCritical() << "Unknown character found in nodename" << nodeName;
            return false;
        }
    }

    // Verify we have a valid number
    bool okay;
    quint64 value = number.toLongLong(&okay);
    if(!okay) {
        return false;
    }


    // Parse Suffix
    while(i < nodeName.constEnd()) {
        suffix.append((*i));
        ++i;
    }

    // Remove .localdomain from the suffix
    if(!suffix.isEmpty() && suffix.endsWith(".localdomain", Qt::CaseInsensitive)) {
        suffix = suffix.remove(".localdomain", Qt::CaseInsensitive);
    }

    // Verify the suffix is valid
    if(this->suffix() != suffix) {
        return false;
    }


    // Check number contained in ranges
    QList<Range*>::iterator r = d->m_Ranges.begin();
    while(r != d->m_Ranges.end()) {
        if((*r)->lower() <= value && value <= (*r)->upper()) {
            return true;
        }
        ++r;
    }

    return false;
}



/*!
   \brief Returns a folded-range string representing the Range
   \return
 */
QString NodeRange::number() const
{
    QStringList rangeList;
    foreach(Range *range, d->m_Ranges) {
        rangeList << range->toString(d->m_RangeWidth);
    }

    if(rangeList.count() <= 0) {
        return QString();
    }

    if(rangeList.count() == 1 && !rangeList.at(0).contains('-')) {
        return rangeList.at(0);
    }

    return QString("[%1]").arg(rangeList.join(","));
}

void NodeRange::setNumber(const QString &number)
{
    d->m_Ranges.clear();
    d->mergeRange(number);
}

QStringList NodeRange::expanded(const int &truncateAt) const
{
    QStringList expandedList;

    int count = 0;

    foreach(Range *range, d->m_Ranges) {
        for(quint64 i = range->lower(); i <= range->upper(); ++i) {
            expandedList << QString("%1%2%3").arg(prefix()).arg(i, d->m_RangeWidth, 10, QChar('0')).arg(suffix());

            if(++count >= truncateAt) {
                return expandedList;
            }
        }
    }

    return expandedList;

}


QStringList NodeRange::semiExpanded() const
{
    QStringList expandedList;

    foreach(Range *range, d->m_Ranges) {
        expandedList << QString("%1[%2]%3").arg(prefix()).arg(range->toString(d->m_RangeWidth)).arg(suffix());
    }

    return expandedList;
}



/*!
   \brief Merges another NodeRange into the Ranges
   \param other
   \return returns true if successful; false otherwise
 */
bool NodeRange::merge(const NodeRange &other)
{
    if(prefix() != other.prefix() || suffix() != other.suffix()) {
        return false;
    }

    foreach(Range *otherRange, other.d->ranges()) {
        bool merged = false;
        foreach(Range *range, d->m_Ranges) {
            if(range->merge(*otherRange)) {
                merged = true;
                break;
            }
        }
        if(!merged) {
            d->m_Ranges.append(otherRange);
        }
    }
    return true;
}

/*!
   \brief Merges a string representation of a nodename into the NodeRange
   \param nodeName
   \return true if successful; false otherwise
 */
bool NodeRange::merge(const QString &nodeName)
{
    QString prefix = "", suffix = "", ranges = "";
    prefix.reserve(nodeName.size());

    //TODO: Deal with IP addresses

    bool error = false;
    QString::const_iterator i = nodeName.constBegin();

    // Parse Prefix
    while(i < nodeName.constEnd() && !i->isNumber() && (*i) != '[') {
        prefix.append((*i));
        ++i;
    }

    if(i < nodeName.constEnd()) {
        // Parse Numeric Range(s)
        if((*i) == '[') {
            if(++i < nodeName.constEnd()) {
                while((*i) != ']') {
                    if((*i) != ' ') {
                        ranges.append((*i));
                    }
                    if(++i >= nodeName.constEnd()) {
                        error = true;
                        break;
                    }
                }
                ++i;
            }

        // Parse Single Number
        } else if(i->isNumber()) {
            QString number;
            do {
                number.append((*i));
                ++i;
            } while(i < nodeName.constEnd() && i->isNumber());
            ranges = number;
        } else {
            qCritical() << "Unknown character found in nodename" << nodeName;
            error = true;
        }
    }

    // Parse Suffix
    while(i < nodeName.constEnd()) {
        suffix.append((*i));
        ++i;
    }

    if(!suffix.isEmpty() && suffix.endsWith(".localdomain", Qt::CaseInsensitive)) {
        suffix = suffix.remove(".localdomain", Qt::CaseInsensitive);
    }

    if(error) { return false; }

    if(!d->m_Initialized) {
        setPrefix(prefix);
        setSuffix(suffix);
        d->m_Initialized = true;
    } else if(this->prefix() != prefix || this->suffix() != suffix) {
        return false;
    }

    foreach(QString range, ranges.split(',')) {
        d->mergeRange(range);
    }

    return true;
}

/*!
   \brief Returns the number of nodes in the range set
   \return
 */
quint64 NodeRange::count() const
{
    quint64 count = 0;
    foreach(Range *range, d->m_Ranges) {
        count += range->count();
    }
    return count;
}

/*!
   \brief Returns a brief list of nodes that is more user friendly than long lists
   \return
 */
QString NodeRange::toShortString() const
{
    QString rangeList;
    if(d->m_Ranges.count() > 2) {
        rangeList = "[";
        rangeList.append(d->m_Ranges.first()->toString(d->m_RangeWidth));
        rangeList.append(",...,");
        rangeList.append(d->m_Ranges.last()->toString(d->m_RangeWidth));
        rangeList.append("]");
    } else {
        rangeList = number();
    }

    rangeList.append(QString(":%1").arg(count()));

    return QString("%1%2%3").arg(prefix()).arg(rangeList).arg(suffix());
}






NodeRangePrivate::NodeRangePrivate() :
    m_RangeWidth(0),
    m_Initialized(false)
{

}

NodeRangePrivate::~NodeRangePrivate()
{
    qDeleteAll(m_Ranges);
}


/*!
   \internal
   \brief Merges a string representation of a value range into this node's range set
   \param rangeString
   \return true if successful; false otherwise
 */
bool NodeRangePrivate::mergeRange(const QString &rangeString) {
    quint64 lower, upper;
    bool lowerOkay = false;
    bool upperOkay = false;

    QString::const_iterator i = rangeString.constBegin();

    QString lowerString, upperString;
    int reserveSize = (rangeString.count() * 0.5) + 2;
    lowerString.reserve(reserveSize);
    upperString.reserve(reserveSize);

    while(i != rangeString.constEnd() && i->isNumber()) {
        lowerString.append((*i));
        ++i;
    }
    m_RangeWidth = qMax(m_RangeWidth, lowerString.count());
    lower = lowerString.toLongLong(&lowerOkay);

    if(i != rangeString.constEnd() && (*i) == '-') {
        ++i;
        while(i != rangeString.constEnd() && i->isNumber()) {
            upperString.append((*i));
            ++i;
        }
        m_RangeWidth = qMax(m_RangeWidth, upperString.count());
        upper = upperString.toLongLong(&upperOkay);
    }

    // Complete failure
    if(!lowerOkay) {
        return false;
    }

    // Really small range  ;-)
    if(/* lowOkay && */ !upperOkay) {
        upper = lower;
    }

    // Oops, swap the range around
    if(lower > upper) {
        quint64 temp = lower;
        lower = upper;
        upper = temp;
    }

    if(!m_Ranges.isEmpty()) {
        // Shortcut to append (most likely case)
        if(lower > m_Ranges.last()->upper() + 1) {
            m_Ranges.append(new Range(lower, upper));
            return true;
        }

        // Shortcut to prepend
        if(upper + 1 < m_Ranges.first()->lower()) {
            m_Ranges.prepend(new Range(lower, upper));
            return true;
        }
    }

    // Check all items for merge capability
    QList<Range*>::iterator r = m_Ranges.begin();
    while(r != m_Ranges.end()) {
        if((*r)->upper() + 1 < lower) {             // Pass over smaller ranges that cannot be merged
            ++r;
            continue;
        } else  if((*r)->lower() > upper + 1) {     // Insert before larger ranges that cannot be merged
            m_Ranges.insert(r, new Range(lower, upper));
            return true;
        } else  if((*r)->merge(lower, upper)) {     // Try to merge at a middling range

            // Forward merge, if possible
            int index = m_Ranges.indexOf(*r) + 1;
            while(index < m_Ranges.count()) {
                if((*r)->merge(*m_Ranges.at(index))) {
                    m_Ranges.removeAt(index);
                } else {
                    break;
                }
            }

            return true;
        }
        ++r;
    }

    // If we get here, simply append it
    m_Ranges.append(new Range(lower, upper));
    return true;
}

/*!
   \internal
   \brief NodeRangePrivate::ranges
   \return
 */
QList<Range*> NodeRangePrivate::ranges() const
{
    return m_Ranges;
}

} // namespace NodeListView
} // namespace Plugins
