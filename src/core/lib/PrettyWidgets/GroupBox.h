/*!
   \file GroupBox.h
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

#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QGroupBox>

#include "Global.h"

class GroupBoxPrivate;

class GroupBox : public QGroupBox
{
    Q_OBJECT
    DECLARE_PRIVATE(GroupBox)

    Q_PROPERTY(bool collapsible READ isCollapsible WRITE setCollapsible)
    Q_PROPERTY(bool collapsed READ isCollapsed WRITE setCollapsed DESIGNABLE isCollapsible NOTIFY collapsed USER true)

public:
    explicit GroupBox(QWidget *parent = 0);
    ~GroupBox();

    virtual QSize minimumSizeHint() const;

    bool isCollapsed() const;

    bool isCollapsible() const;
    void setCollapsible(const bool &collapsible);

signals:
    void collapsed(bool collapsed);

public slots:
    void setCollapsed(const bool &collapse);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GROUPBOX_H
