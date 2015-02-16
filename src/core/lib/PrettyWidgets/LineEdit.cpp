/*!
   \file LineEdit.cpp
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

#include "LineEditPrivate.h"

#if QT_VERSION < 0x040700
#include <QPainter>
#include <QStyle>
#endif


/*! \class LineEdit
    \brief LineEdit

    Goals are to add functionality to QLineEdit.

    * draw help text into box using subdued color
      * do not compile into versions of Qt that already have this functionality

 */

/*!
   \brief LineEdit::LineEdit
   \param parent
 */
LineEdit::LineEdit(QWidget *parent) :
    QLineEdit(parent),
    d(new LineEditPrivate)
{
    d->q = this;
}

/*!
   \internal
   \brief LineEdit::~LineEdit
 */
LineEdit::~LineEdit()
{
}


#if QT_VERSION < 0x040700
/*!
    \property QLineEdit::placeholderText
    \brief the line edit's placeholder text

    Setting this property makes the line edit display a grayed-out
    placeholder text as long as the text() is empty and the widget doesn't
    have focus.

    By default, this property contains an empty string.

    \sa text()
 */
QString LineEdit::placeholderText() const
{
    return d->placeholderText;
}
void LineEdit::setPlaceholderText(const QString& placeholderText)
{
    if (d->placeholderText != placeholderText) {

        d->placeholderText = placeholderText;

        if (!hasFocus()) {
            update();
        }
    }
}
#endif

void LineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

#if QT_VERSION < 0x040700
    // This is a slightly modified version of the source from Qt5.0.2 for this functionality

    if( !(text().isEmpty() && !hasFocus() && !d->placeholderText.isEmpty()) ) {
        return;
    }

    QPainter p(this);
    QRect r = rect();
    QPalette pal = palette();
    QFontMetrics fm = fontMetrics();

    Qt::Alignment va = QStyle::visualAlignment(layoutDirection(), QFlag(alignment()));
    int vscroll = 0;
    switch(va & Qt::AlignVertical_Mask) {
     case Qt::AlignBottom:
         vscroll = r.y() + r.height() - fm.height() - 1;
         break;
     case Qt::AlignTop:
         vscroll = r.y() + 1;
         break;
     default:
         vscroll = r.y() + (r.height() - fm.height() + 1) / 2;
         break;
    }

    QRect lineRect(r.x() + 2, vscroll, r.width() - 4, fm.height());
    int minLB = qMax(0, -fm.minLeftBearing());

    QColor col = pal.text().color();
    col.setAlpha(128);
    QPen oldpen = p.pen();
    p.setPen(col);
    lineRect.adjust(minLB, 0, 0, 0);
    QString elidedText = fm.elidedText(d->placeholderText, Qt::ElideRight, lineRect.width());
    p.drawText(lineRect, va, elidedText);
    p.setPen(oldpen);
#endif

}


/***** PRIVATE IMPLEMENTATION *****/

/*!
   \internal
   \brief LineEditPrivate::LineEditPrivate
 */
LineEditPrivate::LineEditPrivate() :
    QObject(NULL),
    q(NULL)
{
}
