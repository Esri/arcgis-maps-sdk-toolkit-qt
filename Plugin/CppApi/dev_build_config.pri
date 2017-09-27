#-------------------------------------------------
#  Copyright 2017 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

# This is only for internal CI builds with no setup installed

include ($$PWD/../../../../common/qt_api_config.pri)

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

win32:RC_FILE += $$PWD/ArcGISRuntimeToolkit.rc

INCLUDEPATH += ../../../../api/qt_cpp/Include

!ios:LIBS += -L$${DESTDIR} -L$${LIB_FOLDER_STATICLIB} \
             -lEsriRuntimeQt$${LIB_SUFFIX} -lEsriCommonQt$${LIB_SUFFIX}
