###############################################################################
# Copyright 2012-2019 Esri
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
###############################################################################

TEMPLATE = app

QT += core gui opengl network positioning sensors qml quick multimedia

CONFIG += c++17

TARGET = ArExample

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

ARCGIS_RUNTIME_VERSION = 200.6.0
include($$PWD/arcgisruntime.pri)

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc
OTHER_FILES += \
    $$files($$PWD/qml/*.qml) \
    $$files($$PWD/qml/*.js)

#-------------------------------------------------------------------------------
# AR configuration

ARCGIS_TOOLKIT_PATH = $$absolute_path($$PWD/../../)

!include($$ARCGIS_TOOLKIT_PATH/augmented_reality/ArApi/ArApi.pri) {
    error(Failed to open $$ARCGIS_TOOLKIT_PATH/augmented_reality/ArApi/ArApi.pri)
}

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
