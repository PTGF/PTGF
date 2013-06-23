/*!
   \file Range.cpp
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

#include "Range.h"

#include <QString>
#include <QDebug>

namespace Plugins {
namespace NodeListView {

/*!
   \brief Creates a Range with the given upper and lower values
   \param lower
   \param upper
 */
Range::Range(const quint64 &lower, const quint64 &upper)
{
    setValue(lower, upper);
}

/*!
   \brief Convenience function to allow setting both lower and upper values simultaeneously
   \param lower
   \param upper
 */
void Range::setValue(const quint64 &lower, const quint64 &upper)
{
    if(lower > upper) {
        m_Upper = lower;
        m_Lower = upper;
    } else {
        m_Lower = lower;
        m_Upper = upper;
    }
}


/*!
   \brief Property holds the lower range value
   \param lower
 */
quint64 Range::lower() const
{
    return m_Lower;
}
/*!
   \brief Property holds the lower range value
   \param lower
 */
void Range::setLower(const quint64 &lower)
{
    if(lower > m_Upper) {
        m_Lower = m_Upper;
        m_Upper = lower;
    } else {
        m_Lower = lower;
    }
}

/*!
   \brief Property holds the upper range value
 */
quint64 Range::upper() const
{
    return m_Upper;
}
/*!
   \brief Property holds the upper range value
   \param upper
 */
void Range::setUpper(const quint64 &upper)
{
    if(upper < m_Lower) {
       m_Upper = m_Lower;
       m_Lower = upper;
    } else {
        m_Upper = upper;
    }
}

/*!
   \brief Returns a string representation of the range values
   \param width specifies the character width of the returned string (prepended with zeros)
   \return
 */
QString Range::toString(int width) const
{
    QString retval;
    if(m_Lower != m_Upper) {
        retval = QString("%1-%2").arg(m_Lower, width, 10, QChar('0')).arg(m_Upper, width, 10, QChar('0'));
    } else {
        retval = QString("%1").arg(m_Lower, width, 10, QChar('0'));
    }

    return retval;
}

/*!
   \brief Merges another Range into this if possible
   \param other
   \return true if successful; false otherwise
 */
bool Range::merge(const Range &other)
{
    return merge(other.lower(), other.upper());
}

/*!
   \brief merge
   \param range Will be changed, if possible to accomodate the lower/upper pair
   \param lower Lower value to be merged into range
   \param upper Upper value to be merged into range
   \return true if successful merge occured, false otherwise
 */
bool Range::merge(const quint64 &lower, const quint64 &upper)
{
    if(m_Lower <= lower && upper <= m_Upper) {              // Exists completely within the current range
        return true;

    } else if(m_Lower >= lower && upper >= m_Upper) {       // The current range exists completely within
        m_Lower = lower;
        m_Upper = upper;
        return true;

    } else if( (m_Lower <= lower && lower <= m_Upper) ||
               // Assumed safe assumption that we're not using the max possible value of quint64
               (m_Upper + 1 == lower) ) {                     // We can widen current range
        m_Upper = upper;
        return true;

    } else if( (m_Lower <= upper && upper <= m_Upper) ||
               // Assumed safe assumption that we're not using the max possible value of quint64
               (m_Lower == upper + 1) ) {                     // We can widen current range
        m_Lower = lower;
        return true;

    }

    return false;
}
} // namespace NodeListView
} // namespace Plugins
