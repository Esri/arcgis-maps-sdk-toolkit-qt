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

TEMPLATE = app

QT += core gui opengl network positioning sensors qml quick multimedia
CONFIG += c++11

TARGET = CppArSample

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) { 
        error("$$TARGET requires Qt 5.12.0")
    }
	equals(QT_MINOR_VERSION, 12) : lessThan(QT_PATCH_VERSION, 0) {
		error("$$TARGET requires Qt 5.12.0")
	}
}

ARCGIS_RUNTIME_VERSION = 100.6
include($$PWD/arcgisruntime.pri)

HEADERS += \
    AppInfo.h \
    CppArSample.h

SOURCES += \
    main.cpp \
    CppArSample.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

OTHER_FILES += \
    qml/*.qml \
    qml/*.js

#-------------------------------------------------------------------------------

# AR configs

# The path to the AR toolkit must be defined in the AR_TOOLKIT_PATH envrionement variable.
# This path correspond to toolkit, download from GitHub and should be like:
# AR_TOOLKIT_PATH = <path to the toolkit>/toolkit/Plugin/CppApi/

HEADERS += \
    $$AR_TOOLKIT_PATH/include/AR/ArcGISArView.h \
    $$AR_TOOLKIT_PATH/include/AR/ArcGISArViewRenderer.h \
    $$AR_TOOLKIT_PATH/include/AR/ArWrapper.h

SOURCES += \
    $$AR_TOOLKIT_PATH/source/AR/ArcGISArView.cpp \
    $$AR_TOOLKIT_PATH/source/AR/ArcGISArViewRenderer.cpp \
    $$AR_TOOLKIT_PATH/source/AR/ArWrapper.cpp

INCLUDEPATH += $$AR_TOOLKIT_PATH/include/AR

# option to disable the AR in the build?
ios {
    OBJECTIVE_HEADERS += \
        $$AR_TOOLKIT_PATH/include/AR/iOS/ArKitWrapper.h

    OBJECTIVE_SOURCES += \
        $$AR_TOOLKIT_PATH/source/AR/iOS/ArKitWrapper.mm

    HEADERS += \
        $$AR_TOOLKIT_PATH/include/AR/iOS/ArKitFrameRenderer.h \
        $$AR_TOOLKIT_PATH/include/AR/iOS/ArKitPointCloudRenderer.h

    SOURCES += \
        $$AR_TOOLKIT_PATH/source/AR/iOS/ArKitFrameRenderer.cpp \
        $$AR_TOOLKIT_PATH/source/AR/iOS/ArKitPointCloudRenderer.cpp

    INCLUDEPATH += $$AR_TOOLKIT_PATH/include/AR/iOS

    LIBS += -framework ARKit
}

android {
    QT += androidextras

    HEADERS += \
        $$AR_TOOLKIT_PATH/3rdparty/arcore/include/arcore_c_api.h\
        $$AR_TOOLKIT_PATH/include/AR/Android/ArCoreWrapper.h \
        $$AR_TOOLKIT_PATH/include/AR/Android/ArCoreFrameRenderer.h \
        $$AR_TOOLKIT_PATH/include/AR/Android/ArCorePointCloudRenderer.h

    SOURCES += \
        $$AR_TOOLKIT_PATH/source/AR/Android/ArCoreWrapper.cpp \
        $$AR_TOOLKIT_PATH/source/AR/Android/ArCoreFrameRenderer.cpp \
        $$AR_TOOLKIT_PATH/source/AR/Android/ArCorePointCloudRenderer.cpp

    INCLUDEPATH += \
        $$AR_TOOLKIT_PATH/3rdparty/arcore/include/ \
        $$AR_TOOLKIT_PATH/include/AR/Android

    LIBS += -L"/Users/guil8553/applications/qt/sdk/toolkit/Plugin/CppApi/3rdparty/core-1.9.0/jni/arm64-v8a"
    LIBS += -larcore_sdk_c -larcore_sdk_jni

    ANDROID_EXTRA_LIBS += \
        "/Users/guil8553/applications/qt/sdk/toolkit/Plugin/CppApi/3rdparty/core-1.9.0/jni/arm64-v8a/libarcore_sdk_c.so" \
        "/Users/guil8553/applications/qt/sdk/toolkit/Plugin/CppApi/3rdparty/core-1.9.0/jni/arm64-v8a/libarcore_sdk_jni.so"
}

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
