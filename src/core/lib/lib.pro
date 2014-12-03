# This file is part of the Parallel Tools GUI Framework (PTGF)
# Copyright (C) 2010-2013 Argo Navis Technologies, LLC
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

include(lib.pri)

CONFIG(debug, debug|release) {
  TARGET          = CoreD
} else {
  TARGET          = Core
}

win32:target.path = /
else:{
  exists( /usr/lib64 ) {
       message( "Configuring for /lib64 bit library path..." )
       target.path  = /lib64
  } else {
       message( "Configuring for /lib library path..." )
       target.path  = /lib
  }
}

INSTALLS         += target


greaterThan(QT_MAJOR_VERSION, 4) {
    qtHaveModule(xml) {
        QT += xml
    } else {
        DEFINES += NO_XML_MODULE
    }
} else {
    QT += xml
}


###
### Until we solve the issue with QSqlDatabase being destructed before writing settings to disk,
### we'll have to keep this section of code disabled
###
#greaterThan(QT_MAJOR_VERSION, 4) {
#    qtHaveModule(sql) {
#        QT += sql
#    } else {
        DEFINES += NO_SQL_MODULE
#    }
#} else {
#    QT += sql
#}


HEADERS +=  Global.h \
            ActionManager/ActionManager.h \
            ActionManager/ActionManagerLibrary.h \
            ActionManager/ActionManagerPrivate.h \
            ActionManager/Context.h \
            ActionManager/ContextPrivate.h \
            ActionManager/Menu.h \
            CoreWindow/CoreSettingPage.h \
            CoreWindow/CoreWindow.h \
            CoreWindow/CoreWindowLibrary.h \
            NotificationManager/NotificationManager.h \
            NotificationManager/NotificationManagerLibrary.h \
            NotificationManager/NotificationManagerPrivate.h \
            NotificationManager/NotificationWidget.h \
            NotificationManager/NotificationWidgetPrivate.h \
            PluginManager/IPlugin.h \
            PluginManager/PluginManager.h \
            PluginManager/PluginManagerLibrary.h \
            PluginManager/PluginManagerPrivate.h \
            PluginManager/PluginSettingPage.h \
            PluginManager/PluginWrapper.h \
            PrettyWidgets/ButtonBox.h \
            PrettyWidgets/ButtonBoxPrivate.h \
            PrettyWidgets/ConsoleWidget.h \
            PrettyWidgets/ConsoleWidgetPrivate.h \
            PrettyWidgets/GroupBox.h \
            PrettyWidgets/GroupBoxPrivate.h \
            PrettyWidgets/LineEdit.h \
            PrettyWidgets/LineEditPrivate.h \
            PrettyWidgets/TabWidget.h \
            PrettyWidgets/TabWidgetPrivate.h \
            SettingManager/ISettingPage.h \
            SettingManager/ISettingPageFactory.h \
            SettingManager/SettingDialog.h \
            SettingManager/SettingManager.h \
            SettingManager/SettingManagerPrivate.h \
            SettingManager/SettingManagerLibrary.h \
            ViewManager/IView.h \
            ViewManager/IViewFactory.h \
            ViewManager/IViewFilterable.h \
            ViewManager/ViewManager.h \
            ViewManager/ViewManagerLibrary.h \
            ViewManager/ViewManagerPrivate.h \
            WindowManager/AboutDialog.h \
            WindowManager/AboutWidget.h \
            WindowManager/IMainWindow.h \
            WindowManager/WindowManager.h \
            WindowManager/WindowManagerLibrary.h \
            WindowManager/WindowManagerPrivate.h

SOURCES +=  Global.cpp \
            ActionManager/ActionManager.cpp \
            ActionManager/Context.cpp \
            ActionManager/Menu.cpp \
            CoreWindow/CoreSettingPage.cpp \
            CoreWindow/CoreWindow.cpp \
            NotificationManager/NotificationManager.cpp \
            NotificationManager/NotificationWidget.cpp \
            PluginManager/IPlugin.cpp \
            PluginManager/PluginManager.cpp \
            PluginManager/PluginSettingPage.cpp \
            PluginManager/PluginWrapper.cpp \
            PrettyWidgets/ButtonBox.cpp \
            PrettyWidgets/ConsoleWidget.cpp \
            PrettyWidgets/GroupBox.cpp \
            PrettyWidgets/LineEdit.cpp \
            PrettyWidgets/TabWidget.cpp \
            SettingManager/ISettingPage.cpp \
            SettingManager/ISettingPageFactory.cpp \
            SettingManager/SettingDialog.cpp \
            SettingManager/SettingManager.cpp \
            ViewManager/IView.cpp \
            ViewManager/IViewFactory.cpp \
            ViewManager/IViewFilterable.cpp \
            ViewManager/ViewManager.cpp \
            WindowManager/AboutDialog.cpp \
            WindowManager/AboutWidget.cpp \
            WindowManager/IMainWindow.cpp \
            WindowManager/WindowManager.cpp

FORMS   +=  CoreWindow/CoreSettingPage.ui \
            CoreWindow/CoreWindow.ui \
            PluginManager/PluginSettingPage.ui \
            SettingManager/SettingDialog.ui \
            WindowManager/AboutDialog.ui \
            WindowManager/AboutWidget.ui

LIBS    +=

RESOURCES += Resources/Core.qrc

DEFINES          += ACTIONMANAGER_LIBRARY \
                    COREWINDOW_LIBRARY \
                    NOTIFICATIONMANAGER_LIBRARY \
                    PLUGINMANAGER_LIBRARY \
                    SETTINGMANAGER_LIBRARY \
                    VIEWMANAGER_LIBRARY \
                    WINDOWMANAGER_LIBRARY

#debug:DEFINES    += ACTIONMANAGER_DEBUG
#debug:DEFINES    += COREWINDOW_DEBUG
#debug:DEFINES    += NOTIFICATIONMANAGER_DEBUG
#debug:DEFINES    += PLUGINMANAGER_DEBUG
#debug:DEFINES    += SETTINGMANAGER_DEBUG
#debug:DEFINES    += VIEWMANAGER_DEBUG
#debug:DEFINES    += WINDOWMANAGER_DEBUG

OTHER_FILES += CoreWindow/StyleSheet.css
win32: styleSheet.path = /ptgf/
else: styleSheet.path = /share/ptgf/
styleSheet.files = CoreWindow/StyleSheet.css
INSTALLS += styleSheet

coreHeaders.path = /include/core/lib
coreHeaders.files = Global.h
INSTALLS += coreHeaders

actionManagerHeaders.path = /include/core/lib/ActionManager
actionManagerHeaders.files = ActionManager/ActionManagerLibrary.h ActionManager/ActionManager.h ActionManager/Context.h ActionManager/Menu.h
INSTALLS += actionManagerHeaders

notificationManagerHeaders.path = /include/core/lib/NotificationManager
notificationManagerHeaders.files = NotificationManager/NotificationManagerLibrary.h NotificationManager/NotificationManager.h NotificationManager/NotificationWidget.h
INSTALLS += notificationManagerHeaders

pluginManagerHeaders.path = /include/core/lib/PluginManager
pluginManagerHeaders.files = PluginManager/PluginManagerLibrary.h PluginManager/PluginManager.h PluginManager/IPlugin.h
INSTALLS += pluginManagerHeaders

prettyWidgetsHeaders.path = /include/core/lib/PrettyWidgets
prettyWidgetsHeaders.files = PrettyWidgets/ButtonBox.h PrettyWidgets/ConsoleWidget.h PrettyWidgets/GroupBox.h PrettyWidgets/LineEdit.h PrettyWidgets/TabWidget.h
INSTALLS += prettyWidgetsHeaders

settingManagerHeaders.path = /include/core/lib/SettingManager
settingManagerHeaders.files = SettingManager/SettingManagerLibrary.h SettingManager/SettingManager.h SettingManager/ISettingPage.h SettingManager/ISettingPageFactory.h
INSTALLS += settingManagerHeaders

viewManagerHeaders.path = /include/core/lib/ViewManager
viewManagerHeaders.files = ViewManager/ViewManagerLibrary.h ViewManager/ViewManager.h ViewManager/IView.h ViewManager/IViewFactory.h ViewManager/IViewFilterable.h
INSTALLS += viewManagerHeaders

windowManagerHeaders.path = /include/core/lib/WindowManager
windowManagerHeaders.files = WindowManager/WindowManagerLibrary.h WindowManager/WindowManager.h WindowManager/IMainWindow.h
INSTALLS += windowManagerHeaders
