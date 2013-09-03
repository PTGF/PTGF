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
#include <QTimerEvent>


/*! \class TabWidget
    \brief TabWidget
 */

/*!
   \brief TabWidget::TabWidget
   \param parent
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

    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    setAutoUpdateTabTitles(true);
}

/*!
   \internal
   \brief TabWidget::~TabWidget
 */
TabWidget::~TabWidget()
{
}

/*!
   \brief TabWidget::tabBar
   \return
 */
QTabBar *TabWidget::tabBar()
{
    return QTabWidget::tabBar();
}

/*!
   \brief TabWidget::setHideBarOnOne
   \param hide
 */
void TabWidget::setHideBarOnOne(bool hide)
{
    d->m_HideBarOnOne = hide;
    d->updateTabBar();
}

/*!
   \brief TabWidget::hideBarOnOne
   \return
 */
bool TabWidget::hideBarOnOne() const
{
    return d->m_HideBarOnOne;
}

/*!
   \brief TabWidget::setClearStyleSheet
   \param clear
 */
void TabWidget::setClearStyleSheet(bool clear)
{
    d->m_ClearStyleSheet = clear;
    d->updateStyleSheet();
}

/*!
   \brief TabWidget::clearStyleSheet
   \return
 */
bool TabWidget::clearStyleSheet() const
{
    return d->m_ClearStyleSheet;
}

/*!
   \brief TabWidget::setAutoUpdateTabTitles
   \param update
 */
void TabWidget::setAutoUpdateTabTitles(bool update)
{
    if(autoUpdateTabTitles() == update) {
        return;
    }

    if(update) {
        d->m_TabTitleTimer = startTimer(1000);
    } else {
        killTimer(d->m_TabTitleTimer);
        d->m_TabTitleTimer = -1;
    }
}

/*!
   \brief TabWidget::autoUpdateTabTitles
   \return
 */
bool TabWidget::autoUpdateTabTitles() const
{
    return (d->m_TabTitleTimer >= 0);
}





/*!
   \internal
   \brief TabWidget::tabInserted
   \param index
 */
void TabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();

    QTabWidget::tabInserted(index);
}

/*!
   \internal
   \brief TabWidget::tabRemoved
   \param index
 */
void TabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)

    d->updateTabBar();
    d->updateStyleSheet();

    QTabWidget::tabRemoved(index);
}

/*!
   \internal
   \brief TabWidget::timerEvent
   \param event
 */
void TabWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == d->m_TabTitleTimer) {
        d->updateTabTitles();
        event->accept();
        return;
    }

    QObject::timerEvent(event);
}

/*!
   \internal
   \brief TabWidget::showEvent
   \param event
 */
void TabWidget::showEvent(QShowEvent *event)
{
    emit shown();
    QTabWidget::showEvent(event);
}

/*!
   \internal
   \brief TabWidget::hideEvent
   \param event
 */
void TabWidget::hideEvent(QHideEvent *event)
{
    emit hidden();
    QTabWidget::hideEvent(event);
}




/*!
   \brief TabWidget::closeTab
   \param index
 */
void TabWidget::closeTab(int index)
{
    Q_ASSERT(index < count());

    if(index < 0) {
        index = currentIndex();
    }

    QWidget *widget = this->widget(index);
    if(widget && widget->close()) {
        removeTab(index);
        widget->deleteLater();
    }

    emit tabClosed(index);
}



/***** PRIVATE IMPLEMENTATION *****/
/*!
   \internal
   \brief TabWidgetPrivate::TabWidgetPrivate
 */
TabWidgetPrivate::TabWidgetPrivate() :
    QObject(NULL),
    q(NULL)
{
}

/*!
   \internal
   \brief TabWidgetPrivate::updateTabBar
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
   \internal
   \brief TabWidgetPrivate::updateStyleSheet
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

/*!
   \internal
   \brief TabWidgetPrivate::updateTabTitles
 */
void TabWidgetPrivate::updateTabTitles()
{
    for(int i = 0; i < q->count(); ++i) {
        QString newTitle = q->widget(i)->windowTitle();
        if(newTitle != q->tabText(i)) {
            q->setTabText(i, newTitle);
            emit q->tabTitleUpdated(i, newTitle);
        }
    }
}


