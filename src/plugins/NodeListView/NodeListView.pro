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
  TARGET              = NodeListViewD
} else {
  TARGET              = NodeListView
}

SOURCES              += NodeListViewPlugin.cpp \
                        NodeListView.cpp \
                        Slurm.cpp \
                        Range.cpp \
                        Node.cpp \
                        NodeRange.cpp

HEADERS              += NodeListViewPlugin.h \
                        NodeListView.h \
                        NodeListViewLibrary.h \
                        Slurm.h \
                        Range.h \
                        NodeListViewPrivate.h \
                        Node.h \
                        NodePrivate.h \
                        NodeRange.h \
                        NodeRangePrivate.h

DEFINES              += NODELISTVIEW_LIBRARY

nodeListViewPluginHeaders.path = /include/plugins/NodeListView
nodeListViewPluginHeaders.files = NodeListViewLibrary.h NodeListView.h NodeRange.h Node.h Range.h Slurm.h
INSTALLS += nodeListViewPluginHeaders
