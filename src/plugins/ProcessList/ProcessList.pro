# This file is part of the Parallel Tools GUI Framework (PTGF)
# Copyright (C) 2010-2015 Argo Navis Technologies, LLC
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

include(../plugins.pri)

CONFIG(debug, debug|release) {
  TARGET              = ProcessListD
} else {
  TARGET              = ProcessList
}

HEADERS              += ProcessListPlugin.h \
                        ProcessListLibrary.h \
                        ProcessListWidget.h \
                        ProcessListModel.h \
                        ProcessListModelPrivate.h

SOURCES              += ProcessListPlugin.cpp \
                        ProcessListWidget.cpp \
                        ProcessListModel.cpp

FORMS                += ProcessListWidget.ui

DEFINES              += PROCESSLIST_LIBRARY

processListPluginHeaders.path = /include/plugins/ProcessList
processListPluginHeaders.files = ProcessListLibrary.h ProcessListModel.h ProcessListWidget.h
INSTALLS += processListPluginHeaders
