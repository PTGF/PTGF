/*!
   \file TabWidget.h
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

#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

#include "Global.h"

class TabWidgetPrivate;

class TabWidget : public QTabWidget
{
    Q_OBJECT
    DECLARE_PRIVATE(TabWidget)

public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();

    QTabBar *tabBar();

    void setHideBarOnOne(bool hide = true);
    bool hideBarOnOne();

    void setClearStyleSheet(bool clear = true);
    bool clearStyleSheet();

protected:
    virtual void tabInserted(int index);
    virtual void tabRemoved(int index);

};

#endif // TABWIDGET_H
