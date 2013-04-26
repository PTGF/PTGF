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
#include <QProgressBar>

#include <QDebug>

namespace Core {
namespace NotificationManager {

/*! \class NotificationWidget
    \brief Notification bar at the top of the CoreWindow
 */

/*!
   \brief NotificationWidget::NotificationWidget
   \param parent
 */
NotificationWidget::NotificationWidget(QWidget *parent) :
    QFrame(parent),
    d(new NotificationWidgetPrivate)
{
    d->q = this;
    d->setupUi();
}

/*!
   \brief NotificationWidget::NotificationWidget
   \param text
   \param icon
   \param buttons
   \param parent
 */
NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons, QWidget *parent) :
    QFrame(parent),
    d(new NotificationWidgetPrivate)
{
    d->q = this;
    d->setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);
}

/*!
   \brief NotificationWidget::NotificationWidget
   \param text
   \param icon
   \param buttons
   \param receiver
   \param member
   \param parent
 */
NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons,
                                  const QObject *receiver, const char *member, QWidget *parent) :
    QFrame(parent),
    d(new NotificationWidgetPrivate)
{
    d->q = this;
    d->setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);

    if(receiver && member) {
        connect(this, SIGNAL(buttonClicked(StandardButton)), receiver, member);
    }
}

/*!
   \internal
   \brief NotificationWidget::~NotificationWidget
 */
NotificationWidget::~NotificationWidget()
{
}

/*!
   \fn NotificationWidget::text
   \brief This property holds the notification text to be displayed.
   \return
 */
QString NotificationWidget::text() const
{
    return d->m_Label->text();
}

/*!
   \fn NotificationWidget::setText
   \brief This property holds the notification text to be displayed.
   \param text
 */
void NotificationWidget::setText(const QString &text)
{
    d->m_Label->setText(text);
}

/*!
   \fn NotificationWidget::icon
   \brief This property holds the notification widget's icon.
   \return
 */
NotificationWidget::Icon NotificationWidget::icon() const
{
    return d->m_Icon;
}

/*!
   \fn NotificationWidget::setIcon
   \brief This property sets the notification widget's icon.
   \param icon
 */
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

/*!
   \fn NotificationWidget::pixmap
   \brief This property holds the current icon.
   \return
 */
QPixmap NotificationWidget::pixmap() const
{
    return *d->m_IconLabel->pixmap();
}

/*!
   \fn NotificationWidget::setPixmap
   \brief The icon currently used by the notification widget.

   \note that it's often hard to draw one pixmap that looks appropriate in all GUI styles; you may want to
   supply a different pixmap for each platform.

   \param pixmap
 */
void NotificationWidget::setPixmap(const QPixmap &pixmap)
{
    d->m_Icon = NoIcon;
    d->m_IconLabel->setPixmap(pixmap);
}

/*!
   \fn NotificationWidget::standardButtons
   \brief This property holds collection of standard buttons in the message box.
   \return
 */
NotificationWidget::StandardButtons NotificationWidget::standardButtons() const
{
    return StandardButtons(int(d->m_ButtonBox->standardButtons()));
}

/*!
   \fn NotificationWidget::setStandardButtons
   \brief This property controls which standard buttons are used by the message box.
   \param standardButtons
 */
void NotificationWidget::setStandardButtons(StandardButtons standardButtons)
{
    d->m_ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(standardButtons)));
}

/*!
   \internal
   \brief NotificationWidget::on_buttonBox_clicked
   \param button
 */
void NotificationWidget::on_buttonBox_clicked(QAbstractButton *button)
{
    emit buttonClicked((StandardButton)d->m_ButtonBox->standardButton(button));
    this->close();
}

/*!
   \internal
   \brief NotificationWidget::on_closeButton_clicked
 */
void NotificationWidget::on_closeButton_clicked()
{
    emit buttonClicked(Close);
    this->close();
}

/*!
   \fn NotificationWidget::buttons
   \brief Returns a list of buttons associated with this widget
   \return
 */
QList<QAbstractButton *> NotificationWidget::buttons() const
{
    return d->m_ButtonBox->buttons();
}

/*!
   \fn NotificationWidget::addButton(StandardButton button)
   \brief Adds a StandardButton to the notification widget
   \param button
 */
void NotificationWidget::addButton(StandardButton button)
{
    d->m_ButtonBox->addButton((QDialogButtonBox::StandardButton)button);
}

/*!
   \fn NotificationWidget::button
   \brief Returns the button instance associated with the specified StandardButton
   \param button
   \return
 */
QPushButton *NotificationWidget::button(StandardButton button) const
{
    return d->m_ButtonBox->button((QDialogButtonBox::StandardButton)button);
}

/*!
   \fn NotificationWidget::close
   \brief Animates the closing of the widget by shrinking the height over time.

   If the widget is hidden, it is simply closed with no animation.

   If the widget is in the midst of a closing animation, it will simply ignore the close request and continue closing.

   The Qt::WA_DeleteOnClose flag is enabled by default on the NotificationWidget. As per QWidget::close() "if the
   widget has the Qt::WA_DeleteOnClose flag, the widget is also deleted." If this is an undesired behavior, you
   must remove this window flag.

   \return true if the widget was closed; otherwise returns false.
 */

bool NotificationWidget::close()
{
    if(d->m_FadeoutTimerId >= 0) {
        return true;
    }

    emit closing();

    if(!isVisible()) {
        return QFrame::close();
    }

    setEnabled(false);
    setMinimumHeight(height());
    setMaximumHeight(height());

    foreach(QWidget *widget, findChildren<QWidget*>()) {
        widget->hide();
    }

    d->m_FadeoutTimerId = startTimer(25);
    return true;
}


/*!
   \fn NotificationWidget::timeoutInterval
   \brief  This property holds the timeout interval
   \return The interval value that was set for the timeout timer
   \sa setTimeoutInterval
 */
int NotificationWidget::timeoutInterval() const
{
    return d->m_TimeoutInterval;
}

/*!
   \fn NotificationWidget::setTimeoutInterval
   \brief Sets the timeout interval in miliseconds for this notification widget, and begins the timer.
   When the timer expires, the widget will be closed as if the "close" button were pressed.  If a timer has already
   been set, this will stop that timer and create a new one, which starts from the beginning.  If the interval is set
   to zero, the timer will be stopped, and a new one will not be created.
   \param msec
 */
void NotificationWidget::setTimeoutInterval(const int &msec)
{
    // Kill any existing timers
    if(d->m_TimeoutTimerId >= 0) {
        killTimer(d->m_TimeoutTimerId);
        d->m_TimeoutTimerId = -1;
    }

    // create the new timer
    if((d->m_TimeoutInterval = msec) > 0) {
        d->m_TimeoutTimerId = startTimer(d->m_TimeoutInterval);
    }
}

/*!
   \fn NotificationWidget::progress
   \brief This property holds the progress bar's current value.
   \return
 */
int NotificationWidget::progress() const
{
    if(!d->m_ProgressBar) {
        return -1;
    }

    return d->m_ProgressBar->value();

}

/*!
   \fn NotificationWidget::setProgress
   \brief Setting a progress value will add a progress bar to the notification widget.

   Progress bar will be displayed on the left side of the widget.

   Progress can be a value from 0 to 100.  Attempting to change the current value to above the maximum range
   has no effect on the current value.  Setting the value to below the minimum will remove the progress bar.

   \note To remove the progress bar, set the value to less than zero.

   \param progress
 */
void NotificationWidget::setProgress(const int &progress)
{
    if(progress < 0) {
        if(d->m_ProgressBar) {
            d->m_Layout->removeWidget(d->m_ProgressBar);
            d->m_ProgressBar->deleteLater();
            d->m_ProgressBar = NULL;
        }
        return;
    }

    if(!d->m_ProgressBar) {
        d->m_ProgressBar = new QProgressBar(this);
        d->m_ProgressBar->setTextVisible(false);
        d->m_ProgressBar->setMaximumSize(75, 15);
        d->m_ProgressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        d->m_Layout->insertWidget(0, d->m_ProgressBar);
    }

    d->m_ProgressBar->setValue(progress);
}



/*!
   \internal
   \brief NotificationWidget::timerEvent
   \param event
 */
void NotificationWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == d->m_TimeoutTimerId) {
        killTimer(d->m_TimeoutTimerId);
        d->m_TimeoutTimerId = -1;

        emit buttonClicked(Close);
        close();

        event->accept();
        return;

    } else if(event->timerId() == d->m_FadeoutTimerId) {
        // Animate the closing of the widget
        if(height() > 1) {
            setMinimumHeight(height() - 2);
            setMaximumHeight(height() - 2);
        } else {
            killTimer(d->m_FadeoutTimerId);
            d->m_FadeoutTimerId = -1;
            QFrame::close();
        }
        event->accept();
        return;

    }

    QFrame::timerEvent(event);
}


/*!
   \internal
   \brief NotificationWidget::keyReleaseEvent
   \param event
 */
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

NotificationWidgetPrivate::NotificationWidgetPrivate() :
    q(NULL),
    m_Layout(NULL),
    m_Label(NULL),
    m_IconLabel(NULL),
    m_Icon(NotificationWidget::NoIcon),
    m_ButtonBox(NULL),
    m_CloseButton(NULL),
    m_ProgressBar(NULL),
    m_TimeoutInterval(0),
    m_TimeoutTimerId(-1),
    m_FadeoutTimerId(-1)
{
}

void NotificationWidgetPrivate::setupUi()
{
    q->setAttribute(Qt::WA_DeleteOnClose);
    q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    q->setFrameShape(QFrame::Box);

    q->setStyleSheet("QFrame { background-color: rgb(255,255,225); }");

    m_Layout = new QHBoxLayout(q);
    m_Layout->setContentsMargins(9,0,9,0);
    m_Layout->setMargin(0);
    q->setLayout(m_Layout);

    m_IconLabel = new QLabel(q);
    m_IconLabel->setMaximumHeight(16);
    m_IconLabel->setMaximumWidth(16);
    m_IconLabel->setScaledContents(true);
    m_Layout->addWidget(m_IconLabel);

    m_Label = new QLabel(q);
    m_Label->setWordWrap(true);
    m_Label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    m_Layout->addWidget(m_Label);

    m_ButtonBox = new QDialogButtonBox(q);
    m_ButtonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_ButtonBox->setMaximumHeight(16);
    q->connect(m_ButtonBox, SIGNAL(clicked(QAbstractButton*)), q, SLOT(on_buttonBox_clicked(QAbstractButton*)));
    m_Layout->addWidget(m_ButtonBox);

    m_CloseButton = new QToolButton(q);
    m_CloseButton->setMaximumHeight(16);
    m_CloseButton->setMaximumWidth(16);
    m_CloseButton->setAutoRaise(true);
    m_CloseButton->setIcon(QIcon(":/CoreWindow/notificationClose.svg"));
    q->connect(m_CloseButton, SIGNAL(clicked()), q, SLOT(on_closeButton_clicked()));
    m_Layout->addWidget(m_CloseButton);

    q->setFocusPolicy(Qt::StrongFocus);
    q->setFocus();
}



} // namespace CoreWindow
} // namespace Core
