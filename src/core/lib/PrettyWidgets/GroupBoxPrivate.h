/*!
   \file GroupBoxPrivate.h
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

#ifndef GROUPBOXPRIVATE_H
#define GROUPBOXPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//

#include "GroupBox.h"

#include <QObject>

class GroupBoxPrivate : public QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(GroupBox)

public:
    explicit GroupBoxPrivate();

    QRect rectCollapseIcon() const;

    bool m_Collapsible;
    bool m_Collapsed;
    bool m_CollapseIconPressed;

public slots:
    void collapseAnimationFinished();

};

#endif // GROUPBOXPRIVATE_H
