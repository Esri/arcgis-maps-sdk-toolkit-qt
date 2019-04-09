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

# This is only for internal CI builds with no setup installed

include ($$PWD/../../../../common/qt_api_config.pri)

macx: PLATFORM = "macOS"
unix:!macx:!android:!ios: PLATFORM = "linux"
win32: PLATFORM = "windows"
ios: PLATFORM = "iOS"
android: {
  PLATFORM = "android"
  equals(QT_ARCH, i386) {
    ANDROID_ARCH = "x86"
  } else:contains(QT_ARCH, arm64) {
    ANDROID_ARCH = "arm64"
  } else {
    ANDROID_ARCH = "armv7"
  }
}

win32:RC_FILE += $$PWD/ArcGISRuntimeToolkit.rc

INCLUDEPATH += ../../../../api/qt_cpp/Include

!ios:LIBS += -L$${DESTDIR} -L$${LIB_FOLDER_STATICLIB} \
             -lEsriRuntimeQt$${LIB_SUFFIX} -lEsriCommonQt$${LIB_SUFFIX}
