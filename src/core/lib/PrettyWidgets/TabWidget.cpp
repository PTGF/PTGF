/*!
   \file TabWidget.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2011 Argo Navis Technologies, LLC

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

   \section DESCRIPTION

 */

#include "TabWidget.h"
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    m_HideBarOnOne = true;
    m_ClearStyleSheet = true;

    updateTabBar();
    updateStyleSheet();
}


QTabBar *TabWidget::tabBar()
{
    return QTabWidget::tabBar();
}

void TabWidget::setHideBarOnOne(bool hide)
{
    m_HideBarOnOne = hide;
    updateTabBar();
}

bool TabWidget::hideBarOnOne()
{
    return m_HideBarOnOne;
}

void TabWidget::updateTabBar()
{
    if(m_HideBarOnOne) {
        if(count() < 2) {
            tabBar()->hide();
        } else {
            tabBar()->show();
        }
    } else {
        tabBar()->show();
    }
}


void TabWidget::setClearStyleSheet(bool clear)
{
    m_ClearStyleSheet = clear;
    updateStyleSheet();
}

bool TabWidget::clearStyleSheet()
{
    return m_ClearStyleSheet;
}

void TabWidget::updateStyleSheet()
{
    if(!m_ClearStyleSheet || count() < 0) {
        if(!m_StyleSheet.isEmpty()) {
            setStyleSheet(m_StyleSheet);
            m_StyleSheet.clear();
        }
        return;
    }

    if(m_StyleSheet.isEmpty()) {
        m_StyleSheet = styleSheet();
        setStyleSheet(QString());
    }
}


void TabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    updateTabBar();
    updateStyleSheet();
}

void TabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)

    updateTabBar();
    updateStyleSheet();
}
