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

!isEmpty(PLUGINS_PRI_INCLUDED):error("plugins/Plugins.pri already included")
PLUGINS_PRI_INCLUDED = 1

include(../PTGF.pri)

TEMPLATE      = lib
CONFIG       += plugin plugin_with_soname
CONFIG       += depend_includepath

LIBS         += -L$$quote($${BUILD_PATH}/core/lib/$${DIR_POSTFIX}) -lCore$${LIB_POSTFIX}

win32:target.path = /
else:{
  exists( /usr/lib64 ) {
       target.path  = /lib64
  } else {
       target.path  = /lib
  }
}

INSTALLS     += target
