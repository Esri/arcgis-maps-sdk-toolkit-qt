#-------------------------------------------------
#  Copyright 2021 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++14

TEMPLATE = app

SOURCES +=  tst_basemapgalleryitemunittest.cpp

HEADERS += \
    tst_basemapgalleryitemunittest.h

QT += testlib
QT -= gui
QT += core gui opengl xml testlib network positioning sensors multimedia quick

ARCGIS_RUNTIME_VERSION = 100.14

include($$PWD/../arcgisruntime.pri)
include($$PWD/../../../../../uitools/toolkitcpp.pri)
include($$PWD/../../shared/shared.pri)

