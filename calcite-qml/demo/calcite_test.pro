# Copyright 2012-2021 Esri
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

mac {
    cache()
}

#-------------------------------------------------------------------------------

CONFIG += c++14

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick quickcontrols2

ARCGIS_RUNTIME_VERSION = 100.15.0
include($$PWD/arcgisruntime.pri)

TEMPLATE = app
TARGET = calcite_test

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.2")
    }
        equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 2) {
                error("$$TARGET requires Qt 5.15.2")
        }
}

equals(QT_MAJOR_VERSION, 6) {
  error("This version of the ArcGIS Runtime SDK for Qt is incompatible with Qt 6")
}

#-------------------------------------------------------------------------------

SOURCES += \
    main.cpp

# Note: this is how we integrate the Calcite theme into the app binary!
# See `README.md` for setup details.
RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc \
    $$absolute_path($$PWD/../Calcite/calcite.qrc)

OTHER_FILES += \
    wizard.xml \
    wizard.png

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
