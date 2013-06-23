/*!
   \file HostRange.cpp
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

#include "HostRange.h"

#include <QStringList>
#include <QDebug>

namespace Plugins {
namespace NodeListView {

/*!
   \brief Creates a HostRange object from the given hostName string
   \param hostName
 */
HostRange::HostRange(const QString &hostName) :
    m_RangeWidth(0)
{
    merge(hostName);
}

HostRange::~HostRange()
{
    qDeleteAll(m_Ranges);
}


/*!
   \brief Property holds the prefix of the HostRange
 */
QString HostRange::prefix() const
{
    return m_Prefix;
}
/*!
   \brief Property holds the prefix of the HostRange
   \param prefix
 */
void HostRange::setPrefix(const QString &prefix)
{
    m_Prefix = prefix;
}

/*!
   \brief Property holds the suffix of the HostRange
 */
QString HostRange::suffix() const
{
    return m_Suffix;
}
/*!
   \brief Property holds the suffix of the HostRange
   \param suffix
 */
void HostRange::setSuffix(const QString &suffix)
{
    m_Suffix = suffix;
}

QList<Range*> HostRange::ranges() const
{
    return m_Ranges;
}

/*!
   \brief Returns a folded-range string representing the Range
   \return
 */
QString HostRange::range() const
{
    QStringList rangeList;
    foreach(Range *range, m_Ranges) {
        rangeList << range->toString(m_RangeWidth);
    }

    if(rangeList.count() <= 0) {
        return QString();
    }

    if(rangeList.count() == 1 && !rangeList.at(0).contains('-')) {
        return rangeList.at(0);
    }

    return QString("[%1]").arg(rangeList.join(","));
}

/*!
   \brief Merges another HostRange into the Ranges
   \param other
   \return returns true if successful; false otherwise
 */
bool HostRange::merge(const HostRange &other)
{
    if(m_Prefix != other.prefix() || m_Suffix != other.suffix()) {
        return false;
    }

    foreach(Range *otherRange, other.ranges()) {
        bool merged = false;
        foreach(Range *range, m_Ranges) {
            if(range->merge(*otherRange)) {
                merged = true;
                break;
            }
        }
        if(!merged) {
            m_Ranges.append(otherRange);
        }
    }
    return true;
}
/*!
   \brief Returns a range-folded string representing the entire HostRange
   \return
 */
QString HostRange::toString() const
{
    return QString("%1%2%3").arg(prefix()).arg(range()).arg(suffix());
}

/*!
   \brief Returns a full list of expanded nodes
   Returns a truncated list if the host count excedes truncateAt
   \param truncateAt
   \return
 */
QStringList HostRange::expanded(const int &truncateAt) const
{
    QStringList expandedList;

    int count = 0;

    foreach(Range *range, m_Ranges) {
        for(quint64 i = range->lower(); i <= range->upper(); ++i) {
            expandedList << QString("%1%2%3").arg(m_Prefix).arg(i, m_RangeWidth, 10, QChar('0')).arg(m_Suffix);

            if(++count >= truncateAt) {
                return expandedList;
            }
        }
    }

    return expandedList;
}


/*!
   \brief Merges a string representation of a hostname into the HostRange
   \param hostName
   \return true if successful; false otherwise
 */
bool HostRange::merge(const QString &hostName)
{
    QString prefix, suffix, ranges = "";
    prefix.reserve(hostName.size());

    //TODO: Deal with IP addresses

    bool error = false;
    QString::const_iterator i = hostName.constBegin();

    // Parse Prefix
    while(i != hostName.constEnd() && !i->isNumber() && (*i) != '[') {
        prefix.append((*i));
        ++i;
    }

    if(i != hostName.constEnd()) {
        // Parse Numeric Range(s)
        if((*i) == '[') {
            if(++i != hostName.constEnd()) {
                while((*i) != ']') {
                    ranges.append((*i));
                    if(++i == hostName.constEnd()) {
                        qCritical() << "Unexpected format in hostname" << hostName;
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
            } while(i != hostName.constEnd() && i->isNumber());
            ranges = number;
        } else {
            qCritical() << "Unknown character found in hostname" << hostName;
            error = true;
        }
    }

    // Parse Suffix
    while(i != hostName.constEnd() && !i->isNumber()) {
        suffix.append((*i));
        ++i;
    }

    if(error) { return false; }

    if(m_Prefix.isNull() && m_Ranges.isEmpty() && m_Suffix.isNull()) {
        m_Prefix = prefix;
        m_Suffix = suffix;
    } else if(m_Prefix != prefix || m_Suffix != suffix) {
        return false;
    }

    foreach(QString range, ranges.split(',')) {
        mergeRange(range);
    }

    return true;
}

/*!
   \brief Returns the number of hosts in the range set
   \return
 */
quint64 HostRange::count() const
{
    quint64 count = 0;
    foreach(Range *range, m_Ranges) {
        count += range->count();
    }
    return count;
}

/*!
   \brief Merges a string representation of a value range into this host's range set
   \param rangeString
   \return true if successful; false otherwise
 */
bool HostRange::mergeRange(const QString &rangeString) {
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

} // namespace NodeListView
} // namespace Plugins
