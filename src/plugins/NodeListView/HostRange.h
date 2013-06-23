/*!
   \file HostRange.h
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

#ifndef PLUGINS_NODELISTVIEW_HOSTRANGE_H
#define PLUGINS_NODELISTVIEW_HOSTRANGE_H

#include "NodeListViewLibrary.h"
#include <QString>
#include <QList>

#include "Range.h"

namespace Plugins {
namespace NodeListView {

class NODELISTVIEW_EXPORT HostRange {
public:
    HostRange(const QString &hostName);
    ~HostRange();

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QString range() const;

    QString toString() const;

    QStringList expanded(const int &truncateAt = 10000) const;

    bool merge(const HostRange &other);
    bool merge(const QString &hostName);

    quint64 count() const;

protected:
    QList<Range *> ranges() const;
    bool mergeRange(const QString &range);

private:
    QString m_Prefix;
    QList<Range*> m_Ranges;
    QString m_Suffix;
    int m_RangeWidth;
};


} // namespace NodeListView
} // namespace Plugins

#endif // PLUGINS_NODELISTVIEW_HOSTRANGE_H
