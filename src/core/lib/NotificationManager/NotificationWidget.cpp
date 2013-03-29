/*!
   \file NotificationWidget.cpp
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

#include "NotificationWidget.h"
#include "NotificationWidgetPrivate.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QMovie>
#include <QToolButton>
#include <QKeyEvent>

namespace Core {
namespace NotificationManager {

/*! \class NotificationWidget
    \brief Notification bar at the top of the CoreWindow
 */

NotificationWidget::NotificationWidget(QWidget *parent) :
    QFrame(parent),
    d(NULL)
{
    d = new NotificationWidgetPrivate(this);
    d->setupUi();
}

NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons, QWidget *parent) :
    QFrame(parent),
    d(NULL)
{
    d = new NotificationWidgetPrivate(this);
    d->setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);
}

NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons,
                                  const QObject *reciever, const char *member, QWidget *parent) :
    QFrame(parent),
    d(NULL)
{
    d = new NotificationWidgetPrivate(this);
    d->setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);

    if(reciever && member) {
        connect(this, SIGNAL(buttonClicked(StandardButton)), reciever, member);
    }
}

NotificationWidget::~NotificationWidget()
{
}

QString NotificationWidget::text() const
{
    return d->m_Label->text();
}
void NotificationWidget::setText(const QString &text)
{
    d->m_Label->setText(text);
}

NotificationWidget::Icon NotificationWidget::icon() const
{
    return d->m_Icon;
}

void NotificationWidget::setIcon(const Icon &icon)
{
    switch(d->m_Icon = icon) {
    case Information:
        d->m_IconLabel->setPixmap(QPixmap(":/CoreWindow/information.svg"));
        break;
    case Warning:
        d->m_IconLabel->setPixmap(QPixmap(":/CoreWindow/warning.svg"));
        break;
    case Critical:
        d->m_IconLabel->setPixmap(QPixmap(":/CoreWindow/critical.svg"));
        break;
    case Question:
        d->m_IconLabel->setPixmap(QPixmap(":/CoreWindow/question.svg"));
        break;
    case Loading:
    {
        QMovie *movie = new QMovie(":/CoreWindow/loading.gif", QByteArray(), this);
        d->m_IconLabel->setMovie(movie);
        movie->start();
    }
        break;
    default:
        d->m_IconLabel->setPixmap(QPixmap());
    }
}

QPixmap NotificationWidget::pixmap() const
{
    return *d->m_IconLabel->pixmap();
}
void NotificationWidget::setPixmap(const QPixmap &pixmap)
{
    d->m_Icon = NoIcon;
    d->m_IconLabel->setPixmap(pixmap);
}

NotificationWidget::StandardButtons NotificationWidget::standardButtons() const
{
    return StandardButtons(int(d->m_ButtonBox->standardButtons()));
}
void NotificationWidget::setStandardButtons(StandardButtons standardButtons)
{
    d->m_ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(standardButtons)));
}

void NotificationWidget::on_buttonBox_clicked(QAbstractButton *button)
{
    this->hide();
    emit buttonClicked((StandardButton)d->m_ButtonBox->standardButton(button));
    emit closing();
    this->close();
}

void NotificationWidget::on_closeButton_clicked()
{
    this->hide();
    emit buttonClicked(Close);
    emit closing();
    this->close();
}


QList<QAbstractButton *> NotificationWidget::buttons() const
{
    return d->m_ButtonBox->buttons();
}


void NotificationWidget::addButton(StandardButton button)
{
    d->m_ButtonBox->addButton((QDialogButtonBox::StandardButton)button);
}

QPushButton *NotificationWidget::button(StandardButton button) const
{
    return d->m_ButtonBox->button((QDialogButtonBox::StandardButton)button);
}

void NotificationWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->count() == 1) {
        if(event->key() == Qt::Key_Escape) {
            d->m_CloseButton->animateClick();
            return;
        } else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            if(buttons().count() == 0) {
                d->m_CloseButton->animateClick();
                return;
            }
        }
    }

    QFrame::keyReleaseEvent(event);
}



/***** PRIVATE IMPLEMENTATION *****/

NotificationWidgetPrivate::NotificationWidgetPrivate(NotificationWidget *parent) :
    q(parent)
{
}

void NotificationWidgetPrivate::setupUi()
{
    q->setAttribute(Qt::WA_DeleteOnClose);
    q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    q->setFrameShape(QFrame::Box);

    q->setStyleSheet("QFrame { background-color: rgb(255,255,225); }");

    QHBoxLayout *layout = new QHBoxLayout(q);
    layout->setContentsMargins(9,0,9,0);
    layout->setMargin(0);
    q->setLayout(layout);

    m_IconLabel = new QLabel(q);
    m_IconLabel->setMaximumHeight(16);
    m_IconLabel->setMaximumWidth(16);
    m_IconLabel->setScaledContents(true);
    layout->addWidget(m_IconLabel);

    m_Label = new QLabel(q);
    m_Label->setWordWrap(true);
    layout->addWidget(m_Label);

    m_ButtonBox = new QDialogButtonBox(q);
    m_ButtonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_ButtonBox->setMaximumHeight(16);
    q->connect(m_ButtonBox, SIGNAL(clicked(QAbstractButton*)), q, SLOT(on_buttonBox_clicked(QAbstractButton*)));
    layout->addWidget(m_ButtonBox);

    m_CloseButton = new QToolButton(q);
    m_CloseButton->setMaximumHeight(16);
    m_CloseButton->setMaximumWidth(16);
    m_CloseButton->setAutoRaise(true);
    m_CloseButton->setIcon(QIcon(":/CoreWindow/notificationClose.svg"));
    q->connect(m_CloseButton, SIGNAL(clicked()), q, SLOT(on_closeButton_clicked()));
    layout->addWidget(m_CloseButton);

    q->setFocusPolicy(Qt::StrongFocus);
    q->setFocus();
}



} // namespace CoreWindow
} // namespace Core
