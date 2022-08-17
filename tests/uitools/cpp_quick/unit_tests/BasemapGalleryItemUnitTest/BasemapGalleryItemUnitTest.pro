# Copyright 2012-2022 Esri
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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

ARCGIS_RUNTIME_VERSION = 100.15.0

include($$PWD/../arcgisruntime.pri)
include($$PWD/../../../../../uitools/toolkitcpp.pri)
include($$PWD/../../shared/shared.pri)

