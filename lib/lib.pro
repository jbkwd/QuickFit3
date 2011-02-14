TEMPLATE = lib
TARGET = quickfit3lib
DEPENDPATH += .

INCLUDEPATH += ../../../../LIB/trunk/

QT += gui xml
DESTDIR = ../output/
#DLLDESTDIR = $$DESTDIR

# Input
HEADERS += dlgnewproperty.h \
           dlgqffitalgorithmprogressdialog.h \
           programoptions.h \
           qfevaluationeditor.h \
           qfevaluationitem.h \
           qfevaluationitemfactory.h \
           qfevaluationpropertyeditor.h \
           qfpluginevaluation.h \
           qfpluginrawdata.h \
           qfpluginservices.h \
           qfproject.h \
           qfprojectrawdatamodel.h \
           qfprojecttreemodel.h \
           qfproperties.h \
           qfrawdataeditor.h \
           qfrawdatapropertyeditor.h \
           qfrawdatarecord.h \
           qfrawdatarecordfactory.h \
           qfrdrpropertymodel.h \
           qfrdrresultsmodel.h \
           qffitfunction.h \
           qffitalgorithm.h \
           qffitfunctionmanager.h \
           qffitalgorithmmanager.h \
           qfpluginfitfunction.h \
           qfpluginfitalgorithm.h \
           qftools.h \
           qffitalgorithmthreaddedfit.h

SOURCES += dlgnewproperty.cpp \
           dlgqffitalgorithmprogressdialog.cpp \
           programoptions.cpp \
           qfevaluationeditor.cpp \
           qfevaluationitem.cpp \
           qfevaluationitemfactory.cpp \
           qfevaluationpropertyeditor.cpp \
           qfproject.cpp \
           qfprojectrawdatamodel.cpp \
           qfprojecttreemodel.cpp \
           qfproperties.cpp \
           qfrawdataeditor.cpp \
           qfrawdatapropertyeditor.cpp \
           qfrawdatarecord.cpp \
           qfrawdatarecordfactory.cpp \
           qfrdrpropertymodel.cpp \
           qfrdrresultsmodel.cpp \
           qffitalgorithm.cpp \
           qffitfunctionmanager.cpp \
           qffitalgorithmmanager.cpp \
           qftools.cpp \
           qffitalgorithmthreaddedfit.cpp

FORMS +=   dlg_newproperty.ui \
           dlg_fitalgorithmprogress.ui

RESOURCES +=

TRANSLATIONS= ../output/translations/de.ts

INCLUDEPATH +=


MOC_DIR = ./.mocs/
UI_DIR = ./.uis/
RCC_DIR = ./.rccs/
OBJECTS_DIR = ./.objs/
