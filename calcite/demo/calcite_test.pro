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

CONFIG += c++17

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick quickcontrols2

ARCGIS_RUNTIME_VERSION = 200.8.0
include($$PWD/arcgisruntime.pri)

TEMPLATE = app
TARGET = calcite_test

lessThan(QT_MAJOR_VERSION, 6) {
    error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.8.2")
}

equals(QT_MAJOR_VERSION, 6) {
    lessThan(QT_MINOR_VERSION, 8) {
        error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.8.2")
    }
  equals(QT_MINOR_VERSION, 8) : lessThan(QT_PATCH_VERSION, 2) {
    error("This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.8.2")
  }
}

#-------------------------------------------------------------------------------

SOURCES += \
    CalciteTest.cpp \
    main.cpp

HEADERS += \
    CalciteTest.h

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

    # workaround for https://bugreports.qt.io/browse/QTBUG-129651
    # ArcGIS Maps SDK for Qt adds 'QMAKE_RPATHDIR = @executable_path/Frameworks'
    # and ffmpeg frameworks have embedded '@rpath/Frameworks' path.
    # so in order for them to be found, we need to add @executable_path to the
    # search path.
    FFMPEG_LIB_DIR = $$absolute_path($$replace(QMAKE_QMAKE, "qmake6", "../../ios/lib/ffmpeg"))
    FFMPEG_LIB_DIR = $$absolute_path($$replace(FFMPEG_LIB_DIR, "qmake", "../../ios/lib/ffmpeg"))
    QMAKE_LFLAGS += -F$${FFMPEG_LIB_DIR} -Wl,-rpath,@executable_path
    versionAtLeast(QT_VERSION, 6.8.3) {
      FRAMEWORK = "xcframework"
    } else {
      FRAMEWORK = "framework"
    }
    LIBS += -framework libavcodec \
            -framework libavformat \
            -framework libavutil \
            -framework libswresample \
            -framework libswscale
    ffmpeg.files = $${FFMPEG_LIB_DIR}/libavcodec.$${FRAMEWORK} \
                   $${FFMPEG_LIB_DIR}/libavformat.$${FRAMEWORK} \
                   $${FFMPEG_LIB_DIR}/libavutil.$${FRAMEWORK} \
                   $${FFMPEG_LIB_DIR}/libswresample.$${FRAMEWORK} \
                   $${FFMPEG_LIB_DIR}/libswscale.$${FRAMEWORK}
    ffmpeg.path = Frameworks
    QMAKE_BUNDLE_DATA += ffmpeg
}

android {
    include (Android/Android.pri)
}
