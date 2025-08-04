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

ARCGIS_RUNTIME_VERSION = 200.8.0
include($$PWD/arcgisruntime.pri)
include($$PWD/../toolkitcpp/toolkitcpp.pri)

HEADERS += \
  $$files($$PWD/src/*.h)

SOURCES += \
  $$files($$PWD/src/*.cpp)

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
