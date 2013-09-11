/*!
   \file GroupBox.cpp
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

#include "GroupBoxPrivate.h"

#include <QStyleOptionGroupBox>
#include <QStylePainter>
#include <QPaintEvent>
#include <QPropertyAnimation>


/*! \class GroupBox
    \brief GroupBox

    Goals are to add functionality to QGroupBox.

    * icon can be added to top next to title
    * hiding of group via dropdown icon
      * tooltip for dropdown can be set

 */

/*!
   \brief GroupBox::GroupBox
   \param parent
 */
GroupBox::GroupBox(QWidget *parent) :
    QGroupBox(parent),
    d(new GroupBoxPrivate)
{
    d->q = this;

    d->m_CollapseIconPressed = false;
    d->m_Collapsible = false;
    d->m_Collapsed = false;
}

/*!
   \internal
   \brief GroupBox::~GroupBox
 */
GroupBox::~GroupBox()
{
}

/*! \reimp */
void GroupBox::paintEvent(QPaintEvent *event)
{
    QGroupBox::paintEvent(event);

    if(!isCollapsible()) {
        return;
    }

    QRect rectCollapseIcon = d->rectCollapseIcon();
    if(event->rect().intersects(rectCollapseIcon)) {
        QStylePainter paint(this);

        QStyleOptionGroupBox option;
        initStyleOption(&option);

        option.initFrom(this);
        option.rect = rectCollapseIcon;

        if(!isCollapsed()) {
            option.state |= QStyle::State_On;
            option.state &= ~QStyle::State_Off;
        } else {
            option.state |= QStyle::State_Off;
            option.state &= ~QStyle::State_On;
        }

        QStyle::PrimitiveElement primitive = isCollapsed() ? QStyle::PE_IndicatorArrowDown : QStyle::PE_IndicatorArrowUp;

        paint.drawPrimitive(QStyle::PE_IndicatorButtonDropDown, option);

        option.rect = rectCollapseIcon.adjusted(2,2,0,0);
        paint.drawPrimitive(primitive, option);
    }
}

/*! \reimp */
QSize GroupBox::minimumSizeHint() const
{
    QSize size = QGroupBox::minimumSizeHint();

    if(!isCollapsible()) {
        return size;
    }

    size.setWidth(size.width() + d->rectCollapseIcon().width() + 5);
    return size;
}

/*! \reimp */
void GroupBox::mousePressEvent(QMouseEvent *event)
{
    QGroupBox::mousePressEvent(event);

    if(!isCollapsible()) {
        return;
    }

    if(event->button() == Qt::LeftButton) {
        if(d->rectCollapseIcon().contains(event->pos())) {
            d->m_CollapseIconPressed = true;
            event->accept();
        }
    }
}

/*! \reimp */
void GroupBox::mouseReleaseEvent(QMouseEvent *event)
{
    QGroupBox::mouseReleaseEvent(event);

    if(!isCollapsible()) {
        return;
    }

    if(event->button() == Qt::LeftButton) {
        if(d->m_CollapseIconPressed && d->rectCollapseIcon().contains(event->pos())) {
            setCollapsed(!isCollapsed());
            event->accept();
        }
    }

    d->m_CollapseIconPressed = false;
}



/*!
    \property GroupBox::collapsed
    \brief whether the group box has been collapsed

    If this property is true, the group box has been collapsed to just display the title bar.

    This property is false by default.
*/
void GroupBox::setCollapsed(const bool &collapse)
{
    if(!isCollapsible()) {
        return;
    }


    if(collapse == d->m_Collapsed) {
        return;
    }


    QRect geo = geometry();

    if(geo.right() < 0 || geo.bottom() < 0) {
        geo = QRect();
    }

    geo.setSize(geo.size().expandedTo(minimumSizeHint()));

    if(collapse) {
        geo.setHeight(fontMetrics().height() + 1);
    }


#ifndef QT_NO_ANIMATION
    QPropertyAnimation *anim = new QPropertyAnimation(this, "geometry", this);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setDuration(collapse? 250: 500);
    anim->setStartValue(geometry());
    connect(anim, SIGNAL(finished()), d.data(), SLOT(collapseAnimationFinished()));
    anim->setEndValue(geo);

    /*! \note this sometimes causes a flicker, but is necessary for expansion, if we can figure out a way to pause
              event processing for maximum height setting changes, this would fix the flicker problem */
    setMaximumHeight(QWIDGETSIZE_MAX);

    anim->start(QAbstractAnimation::DeleteWhenStopped);

#else
    setGeometry(geo);

#endif

    d->m_Collapsed = collapse;
    emit collapsed(d->m_Collapsed);
}
bool GroupBox::isCollapsed() const
{
    return d->m_Collapsed;
}


/*!
    \property GroupBox::collapsible
    \brief whether the group box can be collapsed

    If this property is true, the group box can be collapsed to just display the title bar.

    This property is false by default.
*/

bool GroupBox::isCollapsible() const
{
    return d->m_Collapsible;
}
void GroupBox::setCollapsible(const bool &collapsible)
{
    d->m_Collapsible = collapsible;

    if(!d->m_Collapsible) {
        setCollapsed(false);
    }
}





/***** PRIVATE IMPLEMENTATION *****/

/*!
   \internal
   \brief GroupBoxPrivate::GroupBoxPrivate
 */
GroupBoxPrivate::GroupBoxPrivate() :
    QObject(NULL),
    q(NULL)
{
}


QRect GroupBoxPrivate::rectCollapseIcon() const
{
    int fontHeight = q->fontMetrics().height();
    return QRect(QPoint(q->width() - (2 * fontHeight) -2, 0), QSize(fontHeight + 4, fontHeight));
}

void GroupBoxPrivate::collapseAnimationFinished()
{
    if(!m_Collapsed) {
        return;
    }

    q->setMaximumHeight(q->geometry().height());
}

