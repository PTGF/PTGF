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

HEADERS +=  ActionManager/ActionManager.h \
            PluginManager/PluginManager.h \
            PluginManager/IPlugin.h \
            PluginManager/PluginWrapper.h \
            SettingManager/SettingManager.h \
            SettingManager/SettingDialog.h \
            SettingManager/ISettingPageFactory.h \
            SettingManager/ISettingPage.h \
            ActionManager/MenuItem.h \
            PluginManager/PluginSettingPage.h \
            CoreWindow/NotificationWidget.h \
            PluginManager/PluginManagerLibrary.h \
            SettingManager/SettingManagerLibrary.h \
            ActionManager/ActionManagerLibrary.h \
            PrettyWidgets/TabWidget.h \
            CoreWindow/CoreSettingPage.h \
            CoreWindow/CoreWindow.h \
            CoreWindow/CoreWindowLibrary.h \
            WindowManager/WindowManager.h \
            WindowManager/IMainWindow.h \
            WindowManager/AboutDialog.h \
            WindowManager/AboutWidget.h \
            WindowManager/WindowManagerLibrary.h \
            NotificationManager/NotificationManager.h \
            NotificationManager/NotificationManagerLibrary.h \
            PrettyWidgets/ConsoleWidget.h

SOURCES +=  ActionManager/ActionManager.cpp \
            PluginManager/PluginManager.cpp \
            PluginManager/IPlugin.cpp \
            PluginManager/PluginWrapper.cpp \
            SettingManager/SettingManager.cpp \
            SettingManager/SettingDialog.cpp \
            SettingManager/ISettingPageFactory.cpp \
            SettingManager/ISettingPage.cpp \
            ActionManager/MenuItem.cpp \
            PluginManager/PluginSettingPage.cpp \
            PrettyWidgets/TabWidget.cpp \
            CoreWindow/CoreWindow.cpp \
            CoreWindow/NotificationWidget.cpp \
            CoreWindow/CoreSettingPage.cpp \
            WindowManager/WindowManager.cpp \
            WindowManager/IMainWindow.cpp \
            WindowManager/AboutDialog.cpp \
            WindowManager/AboutWidget.cpp \
            NotificationManager/NotificationManager.cpp \
            PrettyWidgets/ConsoleWidget.cpp

FORMS   +=  SettingManager/SettingDialog.ui \
            PluginManager/PluginSettingPage.ui \
            CoreWindow/CoreSettingPage.ui \
            CoreWindow/CoreWindow.ui \
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

debug:DEFINES    += COREWINDOW_DEBUG
debug:DEFINES    += WINDOWMANAGER_DEBUG
debug:DEFINES    += ACTIONMANAGER_DEBUG
debug:DEFINES    += PLUGINMANAGER_DEBUG
debug:DEFINES    += SETTINGMANAGER_DEBUG

OTHER_FILES += CoreWindow/StyleSheet.css
win32: styleSheet.path = /ptgf/
else: styleSheet.path = /share/ptgf/
styleSheet.files = CoreWindow/StyleSheet.css
INSTALLS += styleSheet

coreWindowHeaders.path = /include/core/lib/CoreWindow
coreWindowHeaders.files = CoreWindow/CoreWindowLibrary.h CoreWindow/CoreWindow.h CoreWindow/NotificationWidget.h
INSTALLS += coreWindowHeaders

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
prettyWidgetsHeaders.files = PrettyWidgets/TabWidget.h
INSTALLS += prettyWidgetsHeaders
