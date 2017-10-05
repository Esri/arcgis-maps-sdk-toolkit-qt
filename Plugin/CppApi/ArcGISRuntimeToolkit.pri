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

CONFIG(ToolkitBuildUsePrefix): ToolkitPrefix = _dev

CONFIG(debug, debug|release) {
  macx:  DebugSuffix = _debug
  win32: DebugSuffix = d
}

macx: PLATFORM = "macOS"
unix:!macx:!android:!ios: PLATFORM = "linux"
win32: PLATFORM = "windows"
ios: PLATFORM = "iOS"
android: {
  PLATFORM = "android"
  contains(QT_ARCH, i386) {
    ANDROID_ARCH = "x86"
  } else {
    ANDROID_ARCH = "armv7"
  }
}

INCLUDEPATH += $$PWD/include

!android:!win32: PLATFORM_OUTPUT = $$PLATFORM
android: PLATFORM_OUTPUT = $$PLATFORM/$$ANDROID_ARCH
win32: {
  contains(QMAKE_TARGET.arch, x86_64): {
    PLATFORM_OUTPUT = $$PLATFORM/x64
  } else {
    PLATFORM_OUTPUT = $$PLATFORM/x86
  }
}

LIBS += -L$$PWD/output/$$PLATFORM_OUTPUT -lArcGISRuntimeToolkitCppApi$${ToolkitPrefix}$${DebugSuffix}

# unset the previous toolkit path
DEFINES -= ARCGIS_TOOLKIT_IMPORT_PATH=\"$$ARCGIS_TOOLKIT_IMPORT_PATH\"

# Set ArcGIS Runtime Toolkit Path for Qml UI files
ARCGIS_TOOLKIT_IMPORT_PATH = $$absolute_path($$PWD/../../Import)
DEFINES += ARCGIS_TOOLKIT_IMPORT_PATH=\"$$ARCGIS_TOOLKIT_IMPORT_PATH\"

# Add plugin paths to QML_IMPORT_PATH
QML_IMPORT_PATH += $${ARCGIS_TOOLKIT_IMPORT_PATH}

# Add plugin paths to QMLPATHS
QMLPATHS += $${ARCGIS_TOOLKIT_IMPORT_PATH}

# DEFINES
unix:!macx:!ios {
  contains(QMAKE_HOST.os, Linux):{
    # on some linux platforms the string 'linux' is replaced with 1
    # temporarily replace it with ARCGISRUNTIME_SDK_LINUX_REPLACEMENT
    LINUX_PLATFORM_REPLACEMENT = ARCGISRUNTIME_SDK_LINUX_REPLACEMENT
    ARCGIS_TOOLKIT_IMPORT_PATH = $$replace(ARCGIS_TOOLKIT_IMPORT_PATH, linux, $$LINUX_PLATFORM_REPLACEMENT)
    DEFINES += LINUX_PLATFORM_REPLACEMENT=$$LINUX_PLATFORM_REPLACEMENT
  }
}

android {
  ANDROID_EXTRA_LIBS += \
  $$PWD/output/$$PLATFORM_OUTPUT/libArcGISRuntimeToolkitCppApi$${ToolkitPrefix}.so
}
