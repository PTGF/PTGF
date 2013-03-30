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

#include "TabWidgetPrivate.h"

#include <QTabBar>

/*! \class TabWidget
 *  \brief TabWidget
 */

/*!
 * \brief TabWidget::TabWidget
 * \param parent
 */
TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    d(new TabWidgetPrivate)
{
    d->q = this;

    d->m_HideBarOnOne = true;
    d->m_ClearStyleSheet = true;

    d->updateTabBar();
    d->updateStyleSheet();
}

/*!
 * \internal
 * \brief TabWidget::~TabWidget
 */
TabWidget::~TabWidget()
{
}

/*!
 * \brief TabWidget::tabBar
 * \return
 */
QTabBar *TabWidget::tabBar()
{
    return QTabWidget::tabBar();
}

/*!
 * \brief TabWidget::setHideBarOnOne
 * \param hide
 */
void TabWidget::setHideBarOnOne(bool hide)
{
    d->m_HideBarOnOne = hide;
    d->updateTabBar();
}

/*!
 * \brief TabWidget::hideBarOnOne
 * \return
 */
bool TabWidget::hideBarOnOne()
{
    return d->m_HideBarOnOne;
}

/*!
 * \brief TabWidget::setClearStyleSheet
 * \param clear
 */
void TabWidget::setClearStyleSheet(bool clear)
{
    d->m_ClearStyleSheet = clear;
    d->updateStyleSheet();
}

bool TabWidget::clearStyleSheet()
{
    return d->m_ClearStyleSheet;
}

/*!
 * \brief TabWidget::tabInserted
 * \param index
 */
void TabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();
}

/*!
 * \brief TabWidget::tabRemoved
 * \param index
 */
void TabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();
}




/***** PRIVATE IMPLEMENTATION *****/

TabWidgetPrivate::TabWidgetPrivate() :
    QObject(NULL),
    q(NULL)
{
}

/*!
 * \internal
 * \brief TabWidgetPrivate::updateTabBar
 */
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

/*!
 * \internal
 * \brief TabWidgetPrivate::updateStyleSheet
 */
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

