# This file is part of the Open|SpeedShop Graphical User Interface
# Copyright (C) 2010-2011 Argo Navis Technologies, LLC
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

#################
# CONFIGURATION #
#################
QT                += opengl

###########################
# LIBRARY & INCLUDE PATHS #
###########################
isEmpty(SERENE_PATH): error(The SERENE_PATH variable must be set)
isEmpty(SERENE_LIBPATH): SERENE_LIBPATH = $${SERENE_PATH}

!exists($$quote($${SERENE_PATH}/lib/lib.pro)) {
    error(Source files at $$quote($${SERENE_PATH}/lib/) were not found)
}
INCLUDEPATH       += $$quote($${SERENE_PATH}/lib)
DEPENDPATH        += $$quote($${SERENE_PATH}/lib)

win32 {
    POSTFIX = debug  #I don't know how/why this ends up with release only in this library!?
    !exists($$quote($${SERENE_LIBPATH}/lib/$${POSTFIX}/Serene.dll)) {
        error($$quote($${SERENE_LIBPATH}/lib/$${POSTFIX}/Serene.dll) was not found. Please ensure that you have already built the Serene library.)
    }
} else {
    !exists($$quote($${SERENE_LIBPATH}/lib/$${POSTFIX}/libSerene.so)) {
        error($$quote($${SERENE_LIBPATH}/lib/$${POSTFIX}/libSerene.so) was not found. Please ensure that you have already built the Serene library.)
    }
}
LIBS              += -L$$quote($${SERENE_LIBPATH}/lib/$${POSTFIX}) -lSerene