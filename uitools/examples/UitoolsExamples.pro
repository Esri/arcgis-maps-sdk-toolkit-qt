#*******************************************************************************
#  Copyright 2012-2022 Esri
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at

#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
# ******************************************************************************/

TEMPLATE = app
TARGET = UitoolsExamples

CONFIG += c++17 qmltypes

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick

QML_IMPORT_NAME = "DemoApp"
QML_IMPORT_MAJOR_VERSION = 1

lessThan(QT_MAJOR_VERSION, 6) {
    error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.5.6")
}

equals(QT_MAJOR_VERSION, 6) {
    lessThan(QT_MINOR_VERSION, 5) {
        error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.5.6")
    }
  equals(QT_MINOR_VERSION, 5) : lessThan(QT_PATCH_VERSION, 6) {
    error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.5.6")
  }
}

ARCGIS_RUNTIME_VERSION = 200.5.0
include($$PWD/arcgisruntime.pri)
include($$PWD/../toolkitcpp.pri)

HEADERS += \
  $$PWD/src/*.h

SOURCES += \
  $$PWD/src/*.cpp

RESOURCES += \
    qml/qml.qrc \
    qml/demos/demos.qrc \
    $$absolute_path($$PWD/../../calcite/Calcite/calcite.qrc)

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

include($$PWD/src/demos/demos.pri)
include($$PWD/src/proxies/proxies.pri)

#-------------------------------------------------------------------------------

win32 {
    include (Win/Win.pri)
}

macx {
    include (Mac/Mac.pri)
}

ios {
    include (iOS/iOS.pri)
}

android {
    include (Android/Android.pri)
}
