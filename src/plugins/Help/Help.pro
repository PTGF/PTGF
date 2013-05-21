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

HAS_HELP_MODULE = 0
greaterThan(QT_MAJOR_VERSION, 4) {
    qtHaveModule(help) {
        QT     += help
        HAS_HELP_MODULE = 1
    } else {
        warning("")
        warning("Your Qt5 installation didn't include the required QtHelp module.")
        warning("Fedora and Ubuntu default package installations of Qt5 are known to have this issue.")
        warning("The official binary installation packages do work and can be downloaded from <http://qt-project.org/downloads>.")
        warning("Alternatively, you can simply use the (probably) functional Qt4 package provided by your OS.")
        warning("As it stands, we are unable to compile the PTGF help plugin which provides in-GUI help.")
        warning("")
    }
} else {
    CONFIG += help
    HAS_HELP_MODULE = 1
}


isEqual(HAS_HELP_MODULE, 1) {
    include(../plugins.pri)

    CONFIG(debug, debug|release) {
      TARGET           = HelpD
    } else {
      TARGET           = Help
    }

    SOURCES          += HelpPlugin.cpp \
                        HelpWidget.cpp \
                        HelpBrowser.cpp \
                        Settings/SettingPage.cpp

    HEADERS          += HelpPlugin.h \
                        HelpWidget.h \
                        HelpBrowser.h \
                        Settings/SettingPage.h

    FORMS            += Settings/SettingPage.ui

    RESOURCES        += Resources/HelpResources.qrc

    #debug: DEFINES   += HELP_DEBUG
}
