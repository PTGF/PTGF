/*!
   \file CoreWindow.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

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

   \section DESCRIPTION

 */

#include "CoreWindow.h"
#include "ui_CoreWindow.h"

#include <SettingManager/SettingManager.h>
#include <PluginManager/PluginManager.h>

#include "CoreSettingPage.h"

#ifdef COREWINDOW_DEBUG
#  include <QDebug>
#endif


#include <typeinfo>
#include <QtDebug>

namespace Core {
namespace CoreWindow {

/*!
   \class Core::CoreWindow::CoreWindow
   \brief The CoreWindow class is part of the core framework libraries.
   It is the actual viewport that the user sees. All plugins manipulate this view to expose data to the user.

   Singleton class.

   \interface Core::SettingManager::ISettingPageFactory

 */


/*!
   \fn CoreWindow::instance()
   \brief Access to the singleton instance of this class
   \returns A reference to the singleton instance of this class
 */
CoreWindow &CoreWindow::instance()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::instance";
#endif
    static CoreWindow *m_Instance = new CoreWindow();
    return *m_Instance;
}

/*!
   \fn CoreWindow::CoreWindow()
   \brief Constructor.
 */
CoreWindow::CoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CoreWindow),
    m_Initialized(false)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::CoreWindow";
#endif

    ui->setupUi(this);

    initActions();
}

/*!
   \fn CoreWindow::~CoreWindow()
   \brief Destructor.
 */
CoreWindow::~CoreWindow()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::~CoreWindow";
#endif

    delete ui;
}

/*!
   \fn CoreWindow::initialize()
   \brief Initializes this class after it has been constructed.
   This design pattern allows the class to perform any operations after a class (that this object is dependent upon) has been
   constructed.
   \returns true if successful
   \sa initialized(), shutdown()
 */
bool CoreWindow::initialize()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::initialize";
#endif

    try {
        readSettings();

        /* Move the toolbar from the top to the left */
        ui->toolbar->setVisible(true);
        addToolBar(Qt::LeftToolBarArea, ui->toolbar);

        Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
        pluginManager.addObject(this);                         /* Register ourselves as an ISettingPageFactory */
    } catch(...) {
        return false;
    }

    return m_Initialized = true;
}

/*!
   \fn CoreWindow::initialized()
   \brief Returns a boolean value indicating whether this instance has been initialized or not.
   \sa initialize()
 */
bool CoreWindow::initialized()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::initialized";
#endif

    return m_Initialized;
}

/*!
   \fn CoreWindow::shutdown()
   \brief Notifies the instance that it should perform any clean-up operations before destruction.
   This class is called manually, before the application is closed.  It will occur before destruction of the instance.
   \sa initialize()
 */
void CoreWindow::shutdown()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::shutdown";
#endif

    writeSettings();

    deleteLater();
}

/*!
   \fn CoreWindow::readSettings()
   \brief Load settings from the SettingManager.
 */
void CoreWindow::readSettings()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::readSettings";
#endif

    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.beginGroup("CoreWindow");

    QString styleName = settingManager.value("style", QApplication::style()->objectName()).toString();
    QStyle *style = QStyleFactory::create(styleName);
    QApplication::setStyle(style);


    //FIXME: This should not be hard coded here
#ifdef WIN32
    QString filePath = QString("%1/ptgf/StyleSheet.css").arg(QApplication::instance()->applicationDirPath());
#else
    QString filePath = QString("%1/../share/ptgf/StyleSheet.css").arg(QApplication::instance()->applicationDirPath());
#endif
    m_StylesheetFilePath = settingManager.value("styleSheet", m_StylesheetFilePath).toString();
    if(m_StylesheetFilePath.isEmpty()) {
        m_StylesheetFilePath = filePath;
    }

    if(!QFile::exists(m_StylesheetFilePath)) {
        QFileInfo fileInfo(m_StylesheetFilePath);
        notify(tr("Failed to open style sheet: %1<br/>This file path can be changed in the settings").
               arg(fileInfo.absoluteFilePath()), NotificationWidget::Critical);
    }

    QFile styleSheet(m_StylesheetFilePath);
    if(styleSheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(styleSheet.readAll());
        styleSheet.close();
    }

    restoreGeometry(settingManager.value("Geometry").toByteArray());
    restoreState(settingManager.value("State").toByteArray());

    settingManager.endGroup();
}

/*!
   \fn CoreWindow::writeSettings()
   \brief Stores settings in the SettingManager for later retrieval.
 */
void CoreWindow::writeSettings()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::writeSettings";
#endif

    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();

    settingManager.beginGroup("CoreWindow");

    settingManager.setValue("StylesheetFilePath", m_StylesheetFilePath);
    settingManager.setValue("Style", QApplication::style()->objectName());

    settingManager.setValue("Geometry", saveGeometry());
    settingManager.setValue("State", saveState());

    settingManager.endGroup();
}

/*!
   \fn CoreWindow::initActions()
   \returns
 */
void CoreWindow::initActions()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::initActions";
#endif
}

/*!
   \fn CoreWindow::addProgressBar()
   \returns
 */
QProgressBar *CoreWindow::addProgressBar()
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::addProgressBar";
#endif

    QProgressBar *retval = new QProgressBar(this);
    retval->hide();
    retval->setTextVisible(false);
    retval->setMaximumHeight(15);
    retval->setMaximumWidth(75);
    ui->statusbar->insertWidget(0, retval);
    return retval;
}

/*!
   \fn CoreWindow::removeProgressBar()
   \returns
 */
void CoreWindow::removeProgressBar(QProgressBar *progressBar)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::removeProgressBar";
#endif

    progressBar->setParent(NULL);
    ui->statusbar->removeWidget(progressBar);
}

/*!
   \fn CoreWindow::addCentralWidget()
   This function takes ownership of the supplied widget.
   \returns
 */
void CoreWindow::addCentralWidget(QWidget *widget, int priority, QString title, QIcon icon)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::addCentralWidget";
#endif

    int index = ui->stackedWidget->addWidget(widget);

    if(title.isEmpty()) {
        title = widget->windowTitle();
    }

    if(icon.isNull()) {
        icon = widget->windowIcon();
    }

    QAction *selectWidget = new QAction(icon, title, this);
    selectWidget->setData(index);
    selectWidget->setPriority((QAction::Priority)priority);
    selectWidget->setCheckable(true);
    connect(selectWidget, SIGNAL(triggered()), this, SLOT(setCurrentCentralWidget()));

    bool wasInserted = false;
    foreach(QAction *before, ui->toolbar->actions()) {
        int beforePriority = (int)before->priority();
        if(beforePriority > priority) {
            ui->toolbar->insertAction(before, selectWidget);
            wasInserted = true;
            break;
        }
    }

    if(!wasInserted) {
        ui->toolbar->addAction(selectWidget);
    }


    //HACK: Only way to set width expand available space, without re-writing parts of Qt4 internals
    static int maxWidth = 0;
    QToolButton *button = qobject_cast<QToolButton*>(ui->toolbar->widgetForAction(selectWidget));
    if(button->sizeHint().width() > maxWidth) {
        // If the newly added button is biggest, iterate over the others and resize them
        maxWidth = button->sizeHint().width();
        foreach(QAction *action, ui->toolbar->actions()) {
            QToolButton *button = qobject_cast<QToolButton*>(ui->toolbar->widgetForAction(action));
            button->setMinimumWidth(maxWidth);
        }
    } else {
        // Force the width to the largest button size
        button->setMinimumWidth(maxWidth);
    }


    if(ui->toolbar->actions().count() > 0) {
        ui->toolbar->actions().first()->trigger();
    }
}

/*!
   \fn CoreWindow::removeCentralWidget()
   \returns
 */
void CoreWindow::removeCentralWidget(QWidget *widget)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::removeCentralWidget";
#endif

    /* Ensure we own the widget, remove it, and remove the button associated with it */
    for(int index=0; index < ui->stackedWidget->count(); ++index) {
        QWidget *currentWidget = ui->stackedWidget->widget(index);
        if(currentWidget == widget) {
            ui->stackedWidget->removeWidget(widget);
            foreach(QAction *action, ui->toolbar->actions()) {
                if(action->data().toInt() == index) {
                    ui->toolbar->removeAction(action);
                    break;
                }
            }
            break;
        }
    }
}

void CoreWindow::setCurrentCentralWidget(QWidget *current)
{
    /* Ensure that the widget actually exists in our collection, set it as current, and set its associated button to checked */
    for(int index=0; index < ui->stackedWidget->count(); ++index) {
        QWidget *widget = ui->stackedWidget->widget(index);
        if(widget == current) {
            ui->stackedWidget->setCurrentIndex(index);
            foreach(QAction *action, ui->toolbar->actions()) {
                action->setChecked(false);
                if(action->data().toInt() == index) {
                    action->setChecked(true);
                }
            }

            updateWindowTitle();

            break;
        }
    }
}

void CoreWindow::setCurrentCentralWidget()
{
    QObject *object = QObject::sender();
    QAction *action = qobject_cast<QAction *>(object);
    if(action) {
        foreach(QAction *action, ui->toolbar->actions()) {
            action->setChecked(false);
        }

        int index = action->data().toInt();
        ui->stackedWidget->setCurrentIndex(index);
        action->setChecked(true);

        updateWindowTitle();
    }
}

void CoreWindow::updateWindowTitle()
{
    // Set window title appropriately
    if(ui->stackedWidget->currentWidget()->windowTitle() == "Welcome" ||
       ui->stackedWidget->currentWidget()->windowTitle() == "Help") {
        setWindowTitle("Parallel Tools GUI Framework");
        setWindowIcon(QIcon(":/CoreWindow/app.png"));
    } else {
        setWindowTitle(ui->stackedWidget->currentWidget()->windowTitle());
        setWindowIcon(ui->stackedWidget->currentWidget()->windowIcon());

        //TODO: Register for future changes to the currentWidget's title
    }
}

void CoreWindow::on_actionExit_triggered()
{
    close();
}


/*!
   \fn CoreWindow::notify()
   \returns
 */
NotificationWidget *CoreWindow::notify(const QString &text,
                                       NotificationWidget::Icon icon,
                                       NotificationWidget::StandardButtons buttons,
                                       const QObject *reciever, const char *member)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::notify";
#endif

    NotificationWidget *notificationWidget = new NotificationWidget(text, icon, buttons, reciever, member, this);
    ui->centralLayout->insertWidget(0, notificationWidget);
    notificationWidget->setFocus();

    return notificationWidget;
}

QList<QAction*> CoreWindow::allActions()
{
    return allActions(menuBar()->actions());
}

QList<QAction*> CoreWindow::allActions(QList<QAction *> actions)
{
    QList<QAction *> retVal;
    foreach(QAction *action, actions) {
        retVal.append(action);
        if(action->menu()) {
            retVal.append(allActions(action->menu()->actions()));
        }
    }
    return retVal;
}


/*!
   \fn CoreWindow::settingPageIcon()
   \brief Reimplemented from ISettingPageFactory.
   \sa Core::SettingManager::ISettingPageFactory::settingPageIcon()
 */
QIcon CoreWindow::settingPageIcon()
{
    return QIcon(":/CoreWindow/app.png");
}

/*!
   \fn CoreWindow::settingPageName()
   \brief Reimplemented from ISettingPageFactory.
   \reimp Core::SettingManager::ISettingPageFactory::settingPageName()
 */
QString CoreWindow::settingPageName()
{
    return tr("Core");
}

/*!
   \fn CoreWindow::settingPagePriority()
   \brief Reimplemented from ISettingPageFactory.
   \reimp Core::SettingManager::ISettingPageFactory::settingPagePriority()
 */
int CoreWindow::settingPagePriority()
{
    return 10;
}

/*!
   \fn CoreWindow::createSettingPage()
   \brief Reimplemented from ISettingPageFactory.
   \reimp Core::SettingManager::ISettingPageFactory::createSettingPage()
 */
SettingManager::ISettingPage *CoreWindow::createSettingPage()
{
    return new CoreSettingPage();
}


} // namespace CoreWindow
} // namespace Core
