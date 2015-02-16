/*!
   \file Range.h
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

#ifndef PLUGINS_NODELISTVIEW_RANGE_H
#define PLUGINS_NODELISTVIEW_RANGE_H

#include "NodeListViewLibrary.h"

namespace Plugins {
namespace NodeListView {

class NODELISTVIEW_EXPORT Range {
public:
    Range(const quint64 &lower = 0, const quint64 &upper = 0);
    ~Range();

    void setValue(const quint64 &lower, const quint64 &upper);
    void setValue(const quint64 &value);

    quint64 lower() const;
    void setLower(const quint64 &lower);

    quint64 upper() const;
    void setUpper(const quint64 &upper);

    QString toString(int width = -1) const;

    inline bool merge(const Range &other) { return merge(other.lower(), other.upper()); }
    bool merge(const quint64 &lower, const quint64 &upper);

    inline quint64 count() const { return m_Upper - m_Lower + 1; }

    inline bool operator <(const Range &other) {
        // Assumed safe assumption that we're not using the max possible value of a quint64
        return (  m_Upper + 1 < other.lower() );
    }
    inline bool operator >(const Range &other) {
        // Assumed safe assumption that we're not using the max possible value of a quint64
        return ( m_Lower > other.upper() + 1 );
    }
    inline bool isMergable(const Range &other) {
        // Assumed safe assumption that we're not using the max possible value of a quint64
        return ( ( m_Lower <= other.lower() + 1 && other.lower() <= m_Upper + 1 ) ||
                 ( m_Lower <= other.upper() + 1 && other.upper() <= m_Upper + 1 ) ||
                 ( other.lower() <= m_Lower + 1 && m_Lower <= other.upper() + 1 ) ||
                 ( other.lower() <= m_Upper + 1 && m_Upper <= other.upper() + 1 ) );
    }

private:
    quint64 m_Lower;
    quint64 m_Upper;
};

} // namespace NodeListView
} // namespace Plugins

#endif // PLUGINS_NODELISTVIEW_RANGE_H
