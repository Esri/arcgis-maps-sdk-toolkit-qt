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

CONFIG(debug, debug|release) {
  macx:  DebugSuffix = _debug
  win32: DebugSuffix = d
}

macx: PLATFORM = "macOS"
unix:!macx:!android:!ios: PLATFORM = "linux"
win32: PLATFORM = "windows"
ios: PLATFORM = "iOS"
android: PLATFORM = "android"

INCLUDEPATH += $$PWD/include \
               $$PWD/include/CoordinateConversion

win32: {
  contains(QT_ARCH, x86_64): {
    PLATFORM_OUTPUT = $$PLATFORM/x64
  } else {
    PLATFORM_OUTPUT = $$PLATFORM/x86
  }
}
else: {
  PLATFORM_OUTPUT = $$PLATFORM
}

# make sure the local toolkit libs come first on the link line
ARCGIS_RT_SDK_LIBS = $$LIBS

TOOLKIT_SUFFIX = $${ToolkitPrefix}$${DebugSuffix}
android:TOOLKIT_SUFFIX = $${TOOLKIT_SUFFIX}_$${QT_ARCH}
LIBS = -L$$PWD/output/$$PLATFORM_OUTPUT -lArcGISRuntimeToolkitCppApi$${TOOLKIT_SUFFIX} $$ARCGIS_RT_SDK_LIBS

# unset the previous toolkit defines and import paths
DEFINES -= ARCGIS_TOOLKIT_IMPORT_PATH=\"$$ARCGIS_TOOLKIT_IMPORT_PATH\"
QML_IMPORT_PATH -= $${ARCGIS_TOOLKIT_IMPORT_PATH}
QMLPATHS -= $${ARCGIS_TOOLKIT_IMPORT_PATH}

# Set and use the local toolkit defines and import paths
ARCGIS_TOOLKIT_IMPORT_PATH = $$absolute_path($$PWD/../../Import)
DEFINES += ARCGIS_TOOLKIT_IMPORT_PATH=\"$$ARCGIS_TOOLKIT_IMPORT_PATH\"
QML_IMPORT_PATH += $${ARCGIS_TOOLKIT_IMPORT_PATH}
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

android:!CONFIG(TOOLKIT_STATIC_BUILD) {
  ANDROID_EXTRA_LIBS += \
  $$PWD/output/$$PLATFORM_OUTPUT/libArcGISRuntimeToolkitCppApi$${ToolkitPrefix}.so
}
