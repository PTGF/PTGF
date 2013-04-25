/*!
   \file ConsoleWidget.cpp
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

#include "ConsoleWidgetPrivate.h"

#include <QScrollBar>

/*!
   \class ConsoleWidget
   \brief ConsoleWidget
 */



/*! \brief ConsoleWidget::ConsoleWidget
    \param parent
 */
ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QPlainTextEdit(parent),
    d(new ConsoleWidgetPrivate)
{
    d->q = this;

    d->m_DefaultCharFormat.setForeground(palette().foreground());
    d->m_DefaultCharFormat.setFontWeight(QFont::Normal);

    setReadOnly(true);
}

/*! \internal
 */
ConsoleWidget::~ConsoleWidget()
{
}

/*!
   \brief ConsoleWidget::setEventLevelColor
   \param EventLevel
   \param color
 */
void ConsoleWidget::setEventLevelColor(const int &EventLevel, const QColor &color)
{
    QTextCharFormat charFormat;
    charFormat.setForeground(color);
    setEventLevelCharFormat(EventLevel, charFormat);
}

/*!
   \brief ConsoleWidget::setEventLevelCharFormat
   \param EventLevel
   \param charFormat
 */
void ConsoleWidget::setEventLevelCharFormat(const int &EventLevel, const QTextCharFormat &charFormat)
{
    d->m_EventLevelCharFormats[EventLevel] = charFormat;
}

/*!
   \brief ConsoleWidget::messageEvent
   \param eventLevel
   \param message
 */
void ConsoleWidget::messageEvent(const int &eventLevel, const QString &message)
{
    bool wasScrolledToBottom = isScrolledToBottom();

    QString text = message;
    text.remove('\r');
    if(!text.endsWith('\n')) {
        text.append('\n');
    }

    QTextCursor cursor = QTextCursor(document());
    cursor.movePosition(QTextCursor::End);
    cursor.beginEditBlock();
    cursor.insertText(text, d->m_EventLevelCharFormats.value(eventLevel, d->m_DefaultCharFormat));
    cursor.endEditBlock();

    // Scroll to bottom, only if the display was there when we started
    if(wasScrolledToBottom) {
        scrollToBottom();
    }
}

/*!
   \brief ConsoleWidget::isScrolledToBottom
   \return
 */
bool ConsoleWidget::isScrolledToBottom() const
{
    QScrollBar *vert = verticalScrollBar();
    return vert->value() == vert->maximum();
}

/*!
   \brief ConsoleWidget::scrollToBottom
 */
void ConsoleWidget::scrollToBottom()
{
    QScrollBar *vert = verticalScrollBar();
    vert->setValue(vert->maximum());
}

/*!
   \internal
   \brief ConsoleWidget::resizeEvent
   \param event
 */
void ConsoleWidget::resizeEvent(QResizeEvent *event)
{
    bool wasScrolledToBottom = isScrolledToBottom();

    QPlainTextEdit::resizeEvent(event);

    // Remain scrolled to the bottom after the resize event!
    if(wasScrolledToBottom) {
        scrollToBottom();
    }
}





/***** PRIVATE IMPLEMENTATION *****/
/*!
   \class ConsoleWidgetPrivate
   \internal
   \brief ConsoleWidgetPrivate
 */


/*!
   \internal
   \brief ConsoleWidgetPrivate::ConsoleWidgetPrivate
 */
ConsoleWidgetPrivate::ConsoleWidgetPrivate() :
    q(NULL)
{
}
