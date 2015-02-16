/*!
   \file ConsoleWidget.h
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

#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QPlainTextEdit>

#include "Global.h"

class ConsoleWidgetPrivate;

class ConsoleWidget : public QPlainTextEdit
{
    Q_OBJECT
    DECLARE_PRIVATE(ConsoleWidget)

public:
    explicit ConsoleWidget(QWidget *parent = 0);
    ~ConsoleWidget();

    void setEventLevelColor(const int &EventLevel, const QColor &color);
    void setEventLevelCharFormat(const int &EventLevel, const QTextCharFormat &charFormat);

    bool isScrolledToBottom() const;

public slots:
    void messageEvent(const int &eventLevel, const QString &message);

    void scrollToBottom();

protected:
    virtual void resizeEvent(QResizeEvent *event);

};

#endif // CONSOLEWIDGET_H
