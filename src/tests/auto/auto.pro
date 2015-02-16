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

include(../../PTGF.pri)

QT       += testlib
TEMPLATE  = app

CONFIG(debug, debug|release) {
  TARGET = $${APPLICATION_TARGET}AutoTestsD
} else {
  TARGET = $${APPLICATION_TARGET}AutoTests
}


SOURCES  += auto.cpp \
            TestActionManager.cpp \
            TestPluginManager.cpp \
            TestNodeListView.cpp

HEADERS  += TestActionManager.h \
            TestPluginManager.h \
            TestNodeListView.h


LIBS    += -L$$quote($${BUILD_PATH}/core/lib/$${DIR_POSTFIX}) -lCore$${LIB_POSTFIX}
LIBS    += -L$$quote($${BUILD_PATH}/plugins/NodeListView/$${DIR_POSTFIX}) -lNodeListView$${LIB_POSTFIX}

win32:target.path = /
else:target.path  = /bin

INSTALLS += target
