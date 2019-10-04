###############################################################################
# Copyright 2012-2019 Esri
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
###############################################################################

# This configuration file is an internal file.
# To use the AR features of the ArcGIS Runtime Toolkit for Qt,
# uses the files ArCppApi.pri to use C++ API and ArQmlApi.pri
# to use the QML API. See AR.md for details.

AR_COMMON_PATH = $$AR_TOOLKIT_SOURCE_PATH/Common/AR
AR_COMMON_INCLUDE_PATH = $$AR_COMMON_PATH/include
AR_COMMON_SOURCE_PATH = $$AR_COMMON_PATH/source

#-------------------------------------------------
# platform independent configuration

HEADERS += \
    $$AR_COMMON_INCLUDE_PATH/ArcGISArViewInterface.h \
    $$AR_COMMON_INCLUDE_PATH/ArcGISArViewRenderer.h \
    $$AR_COMMON_INCLUDE_PATH/ArWrapper.h \
    $$AR_COMMON_INCLUDE_PATH/SensorStatus.h \
    $$AR_COMMON_INCLUDE_PATH/LocationTrackingMode.h \
    $$AR_COMMON_INCLUDE_PATH/LocationDataSource.h

SOURCES += \
    $$AR_COMMON_SOURCE_PATH/ArcGISArViewInterface.cpp \
    $$AR_COMMON_SOURCE_PATH/ArcGISArViewRenderer.cpp \
    $$AR_COMMON_SOURCE_PATH/ArWrapper.cpp \
    $$AR_COMMON_SOURCE_PATH/LocationDataSource.cpp

INCLUDEPATH += $$AR_COMMON_INCLUDE_PATH
DEPENDPATH += $$AR_COMMON_INCLUDE_PATH

#-------------------------------------------------
# iOS configuration

ios {
    OBJECTIVE_HEADERS += \
        $$AR_COMMON_INCLUDE_PATH/iOS/ArKitWrapper.h

    OBJECTIVE_SOURCES += \
        $$AR_COMMON_SOURCE_PATH/iOS/ArKitWrapper.mm \
        $$AR_COMMON_SOURCE_PATH/iOS/ArKitUtils.mm

    HEADERS += \
        $$AR_COMMON_INCLUDE_PATH/iOS/ArKitFrameRenderer.h \
        $$AR_COMMON_INCLUDE_PATH/iOS/ArKitPointCloudRenderer.h \
        $$AR_COMMON_INCLUDE_PATH/iOS/ArKitPlaneRenderer.h \
        $$AR_COMMON_INCLUDE_PATH/iOS/ArKitUtils.h

    SOURCES += \
        $$AR_COMMON_SOURCE_PATH/iOS/ArKitFrameRenderer.cpp \
        $$AR_COMMON_SOURCE_PATH/iOS/ArKitPointCloudRenderer.cpp \
        $$AR_COMMON_SOURCE_PATH/iOS/ArKitPlaneRenderer.cpp

    INCLUDEPATH += $$AR_COMMON_INCLUDE_PATH/iOS
    DEPENDPATH += $$AR_COMMON_INCLUDE_PATH/iOS

    LIBS += -framework ARKit
}

#-------------------------------------------------
# Android configuration

android {
    QT += androidextras

    HEADERS += \
        $$AR_COMMON_INCLUDE_PATH/Android/ArCoreWrapper.h \
        $$AR_COMMON_INCLUDE_PATH/Android/ArCoreFrameRenderer.h \
        $$AR_COMMON_INCLUDE_PATH/Android/ArCorePointCloudRenderer.h \
        $$AR_COMMON_INCLUDE_PATH/Android/ArCorePlaneRenderer.h

    SOURCES += \
        $$AR_COMMON_SOURCE_PATH/Android/ArCoreWrapper.cpp \
        $$AR_COMMON_SOURCE_PATH/Android/ArCoreFrameRenderer.cpp \
        $$AR_COMMON_SOURCE_PATH/Android/ArCorePointCloudRenderer.cpp \
        $$AR_COMMON_SOURCE_PATH/Android/ArCorePlaneRenderer.cpp

    INCLUDEPATH += $$AR_COMMON_INCLUDE_PATH/Android
    DEPENDPATH += $$AR_COMMON_INCLUDE_PATH/Android

    # Extra files from the ARCore framework (Apache 2.0 licence)
    AR_CORE_PATH = $$AR_COMMON_PATH/3rdparty/arcore
    AR_CORE_INCLUDE_PATH = $$AR_CORE_PATH/include
    AR_CORE_LIBS_PATH = $$AR_CORE_PATH/libs
    AR_CORE_GRADLE_PATH = $$AR_CORE_PATH/gradle

    HEADERS += \
        $$AR_CORE_INCLUDE_PATH/arcore_c_api.h

    INCLUDEPATH += $$AR_CORE_INCLUDE_PATH
    DEPENDPATH += $$AR_CORE_INCLUDE_PATH

    LIBS += -larcore_sdk_c -larcore_sdk_jni

    # libs foreach platforms
    contains(ANDROID_TARGET_ARCH, armeabi-v7a) {
        LIBS += -L"$$AR_CORE_LIBS_PATH/jni/armeabi-v7a"
        ANDROID_EXTRA_LIBS += \
            "$$AR_CORE_LIBS_PATH/jni/armeabi-v7a/libarcore_sdk_c.so" \
            "$$AR_CORE_LIBS_PATH/jni/armeabi-v7a/libarcore_sdk_jni.so"
    }

    contains(ANDROID_TARGET_ARCH, arm64-v8a) {
        LIBS += -L"$$AR_CORE_LIBS_PATH/jni/arm64-v8a"
        ANDROID_EXTRA_LIBS += \
            "$$AR_CORE_LIBS_PATH/jni/arm64-v8a/libarcore_sdk_c.so" \
            "$$AR_CORE_LIBS_PATH/jni/arm64-v8a/libarcore_sdk_jni.so"
    }
}
