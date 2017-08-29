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

TARGET = $$qtLibraryTarget(ArcGISRuntimeToolkitCppApi)
TEMPLATE = lib

QT += core gui opengl network positioning sensors qml quick
CONFIG += c++11

DEFINES += QTRUNTIME_TOOLKIT_BUILD

URI = Esri.ArcGISRuntime.Toolkit
QMAKE_MOC_OPTIONS += -Muri=$$URI

RUNTIME_PRI = arcgis_runtime_qml_cpp.pri
#RUNTIME_PRI = esri_runtime_qt.pri # use this for widgets

ios:RESOURCES += $${PWD}/ArcGISRuntimeToolkit.qrc

ARCGIS_RUNTIME_VERSION = 100.2
include($$PWD/arcgisruntime.pri)

HEADERS += $$PWD/include/*.h
SOURCES += $$PWD/source/*.cpp

INCLUDEPATH +=  $$PWD/include/

CONFIG(release) {
  BUILDTYPE = release
}
else {
  BUILDTYPE = debug
}

DESTDIR = $$PWD/output
OBJECTS_DIR = $$DESTDIR/$$BUILDTYPE/obj
MOC_DIR = $$DESTDIR/$$BUILDTYPE/moc
RCC_DIR = $$DESTDIR/$$BUILDTYPE/qrc
