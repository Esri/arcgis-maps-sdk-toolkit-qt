################################################################################
# Copyright 2012-2017 Esri
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
################################################################################

TARGET = $$qtLibraryTarget(ArcGISRuntimeToolkitComponents)
TEMPLATE = lib

DEFINES += QTRUNTIME_TOOLKIT_COMPONENTS_BUILD

RUNTIME_PRI = arcgis_runtime_qml_cpp.pri
#RUNTIME_PRI = esri_runtime_qt.pri # use this for widgets

ARCGIS_RUNTIME_VERSION = 100.1
include($$PWD/arcgisruntime.pri)

HEADERS += $$PWD/include/*.h
SOURCES += $$PWD/source/*.cpp

INCLUDEPATH +=  $$PWD/include/

DESTDIR = $$PWD/output
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
