TEMPLATE = lib
CONFIG += plugin
TARGET = fitalgorithm_levmar
DEPENDPATH += ./

include(../plugins.pri)
include(../../extlibs/levmar.pri)

DESTDIR = $$QFOUTPUT/plugins

include(../../libquickfitwidgets.pri)

# Input
HEADERS += fitalgorithm_levmar.h \
           levmarconfig.h

SOURCES += fitalgorithm_levmar.cpp \
           levmarconfig.cpp

FORMS +=

RESOURCES +=



TRANSLATIONS= ./translations/de.fitalgorithm_levmar.ts

INCLUDEPATH += ../../lib/ \
               ../../libqf3widgets/ \
               ../../../../../LIB/trunk/ \
               ../../../../../LIB/trunk/qt/ \
               .
SRC_DISTRIBUTED.files = $$HEADERS \
                        $$SOURCES \
                        $$FORMS \
                        $$RESOURCES \
                        $$TRANSLATIONS

QT += core
CONFIG += exceptions rtti stl


