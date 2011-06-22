/*!
   \file
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

   \section LICENSE
   This file is part of the Open|SpeedShop Graphical User Interface
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

#include "WelcomeWidget.h"
#include "ui_WelcomeWidget.h"

#include <QMetaClassInfo>
#include <QDesktopServices>
#include <QDateTime>
#include <PluginManager/PluginManager.h>

#include <QDebug>

namespace Plugins {
namespace Welcome {

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    setObjectName("welcomeWidget");

    connect(&m_UrlMapper, SIGNAL(mapped(int)), this, SLOT(urlClicked(int)));

    /* Check the object pool for anything we should manage */
    Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
    foreach(QObject *object, pluginManager.allObjects()) { pluginObjectRegistered(object); }
    connect(&pluginManager, SIGNAL(objectAdded(QObject*)), this, SLOT(pluginObjectRegistered(QObject*)));
    connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), this, SLOT(pluginObjectDeregistered(QObject*)));

}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::setWelcomeData(const QList<IWelcomeData *> &welcomeData)
{
    m_WelcomeData = welcomeData;

    IWelcomeData *data = m_WelcomeData.first();
    QList<QWidget *> tabs = data->additionalTabs();
    this->layout()->addWidget(tabs.first());
}

void WelcomeWidget::pluginObjectRegistered(QObject *object)
{
    IWelcomeData *welcomeData = qobject_cast<IWelcomeData *>(object);
    if(welcomeData) registerWelcomeData(welcomeData);
}

void WelcomeWidget::pluginObjectDeregistered(QObject *object)
{
    IWelcomeData *welcomeData = qobject_cast<IWelcomeData *>(object);
    if(welcomeData) deregisterWelcomeData(welcomeData);
}

void WelcomeWidget::registerWelcomeData(IWelcomeData *welcomeData)
{
    if(!m_WelcomeData.contains(welcomeData)) {
        m_WelcomeData.append(welcomeData);

        foreach(Link link, welcomeData->actions()) {
            addCommandButton(link, ui->grpActions);
        }

        foreach(Link link, welcomeData->tutorials()) {
            addCommandButton(link, ui->grpTutorials);
        }

        foreach(Link link, welcomeData->examples()) {
            addCommandButton(link, ui->grpExamples);
        }

        foreach(Link link, welcomeData->support()) {
            addCommandButton(link, ui->grpSupport);
        }

        m_TipsAndTricks.append(welcomeData->tipsAndTricks());
        ui->lblTip->setText(m_TipsAndTricks.at(qrand() % m_TipsAndTricks.count()));

        foreach(Link link, welcomeData->recent()) {
            QListWidgetItem *item = new QListWidgetItem(link.title, ui->lstRecent);
            item->setData(Qt::UserRole+1, link.priority);

            item->setToolTip(link.description);
            if(link.url.isValid()) {
                //TODO: Connect where we will handle the url
            } else if(link.receiver && link.method) {
                //TODO: Handle the click event
            }

            //TODO: sort by priority (which should be the time it was last accessed)
            ui->lstRecent->addItem(item);
        }

        foreach(QWidget *tab, welcomeData->additionalTabs()) {
            ui->tabWidget->addTab(tab, tab->windowIcon(), tab->windowTitle());
        }
    }
}

void WelcomeWidget::addCommandButton(const Link &link, QWidget *parent)
{
    QCommandLinkButton *btn = new QCommandLinkButton(link.title, link.description, parent);

    if(link.url.isValid()) {
        connect(btn, SIGNAL(clicked()), &m_UrlMapper, SLOT(map()));
        m_UrlMapper.setMapping(btn, m_Urls.count());
        m_Urls.append(link.url);
    } else if(link.receiver && link.method) {
        connect(btn, SIGNAL(clicked()), link.receiver, link.method);
    }

    switch(link.type) {
    case Link::Type_Text:
        btn->setIcon(QIcon(":/Welcome/text.svg"));
        break;
    case Link::Type_Video:
        btn->setIcon(QIcon(":/Welcome/video.svg"));
        break;
    case Link::Type_Web:
        btn->setIcon(QIcon(":/Welcome/link.svg"));
        break;
    default:
         /* Gets rid of stupid compiler warnings; it's like GNU expects coders to not understand switch statements. */
        break;
    }

    /* Insert the item into the parent based on priority */
    bool insertedWidget = false;
    btn->setProperty("priority", link.priority);
    QBoxLayout *layout = qobject_cast<QBoxLayout *>(parent->layout());
    for(int index=0; index < layout->count(); ++index) {
        QLayoutItem *layoutItem = layout->itemAt(index);
        QWidget *widgetItem = layoutItem->widget();
        bool okay;
        if(widgetItem) {
            int itemPriority = widgetItem->property("priority").toInt(&okay);
            if(okay && itemPriority > link.priority) {
                layout->insertWidget(index, btn);
                insertedWidget = true;
                break;
            }
        }
    }

    if(!insertedWidget) {
        layout->addWidget(btn);
    }
}

void WelcomeWidget::deregisterWelcomeData(IWelcomeData *welcomeData)
{
    if(m_WelcomeData.contains(welcomeData)) {
        m_WelcomeData.removeOne(welcomeData);

        /* Remove the data from the widget */
    }
}

void WelcomeWidget::urlClicked(int index)
{
    QUrl url = m_Urls.at(index);
    if(url.scheme() == "qthelp") {
        //TODO: Focus on the help widget, and goto the help url
    } else {
        QDesktopServices::openUrl(url);
    }
}

void WelcomeWidget::setCurrentTip(int index) {
    if(index >= m_TipsAndTricks.count()) {
        m_CurrentTip = 0;
    } else if(index < 0) {
        m_CurrentTip = m_TipsAndTricks.count()-1;
    } else {
        m_CurrentTip = index;
    }

    ui->lblTip->setText(m_TipsAndTricks.at(m_CurrentTip));
}

void WelcomeWidget::randomTip()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    setCurrentTip(qrand() % m_TipsAndTricks.count());
}

void WelcomeWidget::on_btnTipNext_clicked()
{
    setCurrentTip(m_CurrentTip+1);
}

void WelcomeWidget::on_btnTipPrevious_clicked()
{
    setCurrentTip(m_CurrentTip-1);
}

} // namespace Welcome
} // namespace Plugins