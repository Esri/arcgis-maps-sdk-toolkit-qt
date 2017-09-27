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

LIBS += -L$$PWD/output/$$PLATFORM_OUTPUT -lArcGISRuntimeToolkitCppApi
