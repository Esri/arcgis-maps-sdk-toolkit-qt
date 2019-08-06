#-------------------------------------------------
#  Copyright 2019 ESRI
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

# This configuration file must be added in a project to use the AR features of
# the ArcGIS Runtime Toolkit for Qt.

#-------------------------------------------------
# plateform independant configuration

HEADERS += \
    $$PWD/CppApi/include/AR/ArcGISArViewInterface.h \
    $$PWD/CppApi/include/AR/ArcGISArViewRenderer.h \
    $$PWD/CppApi/include/AR/ArWrapper.h \
    $$PWD/CppApi/include/AR/SensorStatus.h \
    $$PWD/CppApi/include/AR/ArKitUsage.h \
    $$PWD/CppApi/include/AR/LocationDataSource.h \
    $$PWD/CppApi/include/AR/OrientationDataSource.h

SOURCES += \
    $$PWD/CppApi/source/AR/ArcGISArViewInterface.cpp \
    $$PWD/CppApi/source/AR/ArcGISArViewRenderer.cpp \
    $$PWD/CppApi/source/AR/ArWrapper.cpp \
    $$PWD/CppApi/source/AR/LocationDataSource.cpp \
    $$PWD/CppApi/source/AR/OrientationDataSource.cpp

INCLUDEPATH += $$PWD/CppApi/include/AR

#-------------------------------------------------
# iOS configuration

ios {
    OBJECTIVE_HEADERS += \
        $$PWD/CppApi/include/AR/iOS/ArKitWrapper.h

    OBJECTIVE_SOURCES += \
        $$PWD/CppApi/source/AR/iOS/ArKitWrapper.mm

    HEADERS += \
        $$PWD/CppApi/include/AR/iOS/ArKitFrameRenderer.h \
        $$PWD/CppApi/include/AR/iOS/ArKitPointCloudRenderer.h \
        $$PWD/CppApi/include/AR/iOS/ArKitPlaneRenderer.h \
        $$PWD/CppApi/include/AR/iOS/ArKitUtils.h

    SOURCES += \
        $$PWD/CppApi/source/AR/iOS/ArKitFrameRenderer.cpp \
        $$PWD/CppApi/source/AR/iOS/ArKitPointCloudRenderer.cpp \
        $$PWD/CppApi/source/AR/iOS/ArKitPlaneRenderer.cpp

    INCLUDEPATH += $$PWD/CppApi/include/AR/iOS

    LIBS += -framework ARKit
}

#-------------------------------------------------
# Android configuration

android {
    QT += androidextras

    HEADERS += \
        $$AR_CORE_HEADER_PATH/arcore_c_api.h \
        $$PWD/CppApi/include/AR/Android/ArCoreWrapper.h \
        $$PWD/CppApi/include/AR/Android/ArCoreFrameRenderer.h \
        $$PWD/CppApi/include/AR/Android/ArCorePointCloudRenderer.h \
        $$PWD/CppApi/include/AR/Android/ArCorePlaneRenderer.h

    SOURCES += \
        $$PWD/CppApi/source/AR/Android/ArCoreWrapper.cpp \
        $$PWD/CppApi/source/AR/Android/ArCoreFrameRenderer.cpp \
        $$PWD/CppApi/source/AR/Android/ArCorePointCloudRenderer.cpp \
        $$PWD/CppApi/source/AR/Android/ArCorePlaneRenderer.cpp

    INCLUDEPATH += \
        $$AR_CORE_HEADER_PATH \
        $$PWD/CppApi/include/AR/Android

    LIBS += -L"$$AR_CORE_BINARIES_PATH/jni/arm64-v8a"
    LIBS += -larcore_sdk_c -larcore_sdk_jni

    ANDROID_EXTRA_LIBS += \
        "$$AR_CORE_BINARIES_PATH/jni/arm64-v8a/libarcore_sdk_c.so" \
        "$$AR_CORE_BINARIES_PATH/jni/arm64-v8a/libarcore_sdk_jni.so"
}
