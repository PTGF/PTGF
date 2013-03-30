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
else:target.path  = /lib
INSTALLS         += target

HEADERS +=  Global.h \
            ActionManager/ActionManager.h \
            ActionManager/ActionManagerLibrary.h \
            ActionManager/ActionManagerPrivate.h \
            ActionManager/MenuItem.h \
            ActionManager/Context.h \
            ActionManager/ContextPrivate.h \
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
            PrettyWidgets/ConsoleWidget.h \
            PrettyWidgets/ConsoleWidgetPrivate.h \
            PrettyWidgets/TabWidget.h \
            PrettyWidgets/TabWidgetPrivate.h \
            SettingManager/ISettingPage.h \
            SettingManager/ISettingPageFactory.h \
            SettingManager/SettingDialog.h \
            SettingManager/SettingManager.h \
            SettingManager/SettingManagerPrivate.h \
            SettingManager/SettingManagerLibrary.h \
            WindowManager/AboutDialog.h \
            WindowManager/AboutWidget.h \
            WindowManager/IMainWindow.h \
            WindowManager/WindowManager.h \
            WindowManager/WindowManagerLibrary.h \
            WindowManager/WindowManagerPrivate.h

SOURCES +=  Global.cpp \
            ActionManager/ActionManager.cpp \
            ActionManager/Context.cpp \
            ActionManager/MenuItem.cpp \
            CoreWindow/CoreSettingPage.cpp \
            CoreWindow/CoreWindow.cpp \
            NotificationManager/NotificationManager.cpp \
            NotificationManager/NotificationWidget.cpp \
            PluginManager/IPlugin.cpp \
            PluginManager/PluginManager.cpp \
            PluginManager/PluginSettingPage.cpp \
            PluginManager/PluginWrapper.cpp \
            PrettyWidgets/ConsoleWidget.cpp \
            PrettyWidgets/TabWidget.cpp \
            SettingManager/ISettingPage.cpp \
            SettingManager/ISettingPageFactory.cpp \
            SettingManager/SettingDialog.cpp \
            SettingManager/SettingManager.cpp \
            WindowManager/AboutDialog.cpp \
            WindowManager/AboutWidget.cpp \
            WindowManager/IMainWindow.cpp \
            WindowManager/WindowManager.cpp \

FORMS   +=  CoreWindow/CoreSettingPage.ui \
            CoreWindow/CoreWindow.ui \
            PluginManager/PluginSettingPage.ui \
            SettingManager/SettingDialog.ui \
            WindowManager/AboutDialog.ui \
            WindowManager/AboutWidget.ui

LIBS    +=

RESOURCES += Resources/Core.qrc

DEFINES          += COREWINDOW_LIBRARY \
                    PLUGINMANAGER_LIBRARY \
                    SETTINGMANAGER_LIBRARY \
                    ACTIONMANAGER_LIBRARY \
                    WINDOWMANAGER_LIBRARY \
                    NOTIFICATIONMANAGER_LIBRARY

#debug:DEFINES    += COREWINDOW_DEBUG
#debug:DEFINES    += WINDOWMANAGER_DEBUG
#debug:DEFINES    += ACTIONMANAGER_DEBUG
#debug:DEFINES    += PLUGINMANAGER_DEBUG
#debug:DEFINES    += SETTINGMANAGER_DEBUG

OTHER_FILES += CoreWindow/StyleSheet.css
win32: styleSheet.path = /ptgf/
else: styleSheet.path = /share/ptgf/
styleSheet.files = CoreWindow/StyleSheet.css
INSTALLS += styleSheet

coreHeaders.path = /include/core/lib
coreHeaders.files = Global.h
INSTALLS += coreHeaders

notificationManagerHeaders.path = /include/core/lib/NotificationManager
notificationManagerHeaders.files = NotificationManager/NotificationManagerLibrary.h NotificationManager/NotificationManager.h NotificationManager/NotificationWidget.h
INSTALLS += notificationManagerHeaders

windowManagerHeaders.path = /include/core/lib/WindowManager
windowManagerHeaders.files = WindowManager/WindowManagerLibrary.h WindowManager/WindowManager.h WindowManager/IMainWindow.h
INSTALLS += windowManagerHeaders

pluginManagerHeaders.path = /include/core/lib/PluginManager
pluginManagerHeaders.files = PluginManager/PluginManagerLibrary.h PluginManager/PluginManager.h PluginManager/IPlugin.h
INSTALLS += pluginManagerHeaders

settingManagerHeaders.path = /include/core/lib/SettingManager
settingManagerHeaders.files = SettingManager/SettingManagerLibrary.h SettingManager/SettingManager.h SettingManager/ISettingPage.h SettingManager/ISettingPageFactory.h
INSTALLS += settingManagerHeaders

actionManagerHeaders.path = /include/core/lib/ActionManager
actionManagerHeaders.files = ActionManager/ActionManagerLibrary.h ActionManager/ActionManager.h
INSTALLS += actionManagerHeaders

prettyWidgetsHeaders.path = /include/core/lib/PrettyWidgets
prettyWidgetsHeaders.files = PrettyWidgets/TabWidget.h PrettyWidgets/ConsoleWidget.h
INSTALLS += prettyWidgetsHeaders
