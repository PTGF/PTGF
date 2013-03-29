/*!
   \file TabWidget.cpp
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

#include "TabWidget.h"
#include "TabWidgetPrivate.h"

#include <QTabBar>

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    d(NULL)
{
    d = new TabWidgetPrivate(this);

    d->m_HideBarOnOne = true;
    d->m_ClearStyleSheet = true;

    d->updateTabBar();
    d->updateStyleSheet();
}

QTabBar *TabWidget::tabBar()
{
    return QTabWidget::tabBar();
}

void TabWidget::setHideBarOnOne(bool hide)
{
    d->m_HideBarOnOne = hide;
    d->updateTabBar();
}

bool TabWidget::hideBarOnOne()
{
    return d->m_HideBarOnOne;
}

void TabWidget::setClearStyleSheet(bool clear)
{
    d->m_ClearStyleSheet = clear;
    d->updateStyleSheet();
}

bool TabWidget::clearStyleSheet()
{
    return d->m_ClearStyleSheet;
}

void TabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();
}

void TabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();
}




/***** PRIVATE IMPLEMENTATION *****/

TabWidgetPrivate::TabWidgetPrivate(TabWidget *parent) :
    QObject(parent),
    q(parent)
{
}


void TabWidgetPrivate::updateTabBar()
{
    if(m_HideBarOnOne) {
        if(q->count() < 2) {
            q->tabBar()->hide();
        } else {
            q->tabBar()->show();
        }
    } else {
        q->tabBar()->show();
    }
}

void TabWidgetPrivate::updateStyleSheet()
{
    if(!m_ClearStyleSheet || q->count() < 0) {
        if(!m_StyleSheet.isEmpty()) {
            q->setStyleSheet(m_StyleSheet);
            m_StyleSheet.clear();
        }
        return;
    }

    if(m_StyleSheet.isEmpty()) {
        m_StyleSheet = q->styleSheet();
        q->setStyleSheet(QString());
    }
}

