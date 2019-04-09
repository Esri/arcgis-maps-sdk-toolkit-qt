################################################################################
#  Copyright 2012-2018 Esri
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
################################################################################

TARGET = $$qtLibraryTarget(ArcGISRuntimeToolkitCppApi$${ToolkitPrefix})
TEMPLATE = lib

QT += core gui opengl network positioning sensors qml quick
CONFIG += c++11 plugin

DEFINES += QTRUNTIME_TOOLKIT_BUILD

HEADERS += $$PWD/include/*.h \
           $$PWD/include/CoordinateConversion/*.h
SOURCES += $$PWD/source/*.cpp \
           $$PWD/source/CoordinateConversion/*.cpp

INCLUDEPATH += $$PWD/include/ \
               $$PWD/include/CoordinateConversion/

RUNTIME_PRI = arcgis_runtime_qml_cpp.pri
#RUNTIME_PRI = esri_runtime_qt.pri # use this for widgets
ARCGIS_RUNTIME_VERSION = 100.5

!CONFIG(daily) {
  include($$PWD/arcgisruntime.pri)
} else {
  include($$PWD/dev_build_config.pri)
}

unix:!macx:!android:!ios: {
  # Linux: make sure we get the definition of std::__throw_bad_function_call from the
  # standard library since libQt5Qml.so.5.6.2 also defines it.
  LIBS += -lstdc++
}

ios {
  QMAKE_CXXFLAGS += -fembed-bitcode
}

macx: {
  QMAKE_POST_LINK =
}

CONFIG(release, debug|release) {
  BUILDTYPE = release
} else {
  BUILDTYPE = debug
}

!android:!win32: PLATFORM_OUTPUT = $$PLATFORM
android: PLATFORM_OUTPUT = $$PLATFORM/$$ANDROID_ARCH
win32: {
  contains(QMAKE_TARGET.arch, x86_64): {
    PLATFORM_OUTPUT = $$PLATFORM/x64
  } else {
    PLATFORM_OUTPUT = $$PLATFORM/x86
  }
}

DESTDIR = $$PWD/output/$$PLATFORM_OUTPUT
OBJECTS_DIR = $$DESTDIR/$$BUILDTYPE/obj
MOC_DIR = $$DESTDIR/$$BUILDTYPE/moc
RCC_DIR = $$DESTDIR/$$BUILDTYPE/qrc
