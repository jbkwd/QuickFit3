#
#    Copyright (c) 2008-2014 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center
#	
#	 last modification: $LastChangedDate$  (revision $Rev$)
#
#    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).
#
#    This software is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

TEMPLATE = lib
CONFIG += plugin

TARGET = fitfunctions_2ffcs
DEPENDPATH += ./

include(../plugins.pri)
DESTDIR = $$QFOUTPUT/plugins

# Input
HEADERS += qfpfitfunctions2ffcs.h \
           qffitfunction2ffccsnormaldiff3d.h \
    qffitfunction2ffccsnormaldiff3dflow.h \
    qffitfunction2ffccsnormaldiff2d.h \
    qffitfunction2ffccsnormaldiff2dflow.h

SOURCES += qfpfitfunctions2ffcs.cpp\
           qffitfunction2ffccsnormaldiff3d.cpp \
    qffitfunction2ffccsnormaldiff3dflow.cpp \
    qffitfunction2ffccsnormaldiff2d.cpp \
    qffitfunction2ffccsnormaldiff2dflow.cpp

FORMS =

RESOURCES +=

TRANSLATIONS= ./translations/de.fitfunctions_2ffcs.ts


INCLUDEPATH += ../../lib/


QT += core
CONFIG += exceptions rtti stl

SRC_DISTRIBUTED.files = $$HEADERS \
                        $$SOURCES \
                        $$FORMS \
                        $$RESOURCES \
                        $$TRANSLATIONS \
                        $$GPL3LICENSE


