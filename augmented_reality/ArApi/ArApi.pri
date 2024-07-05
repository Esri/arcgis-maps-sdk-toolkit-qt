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
# uses the files ArApi.pri to use C++ API. See AR.md for details.

isEmpty(ARCGIS_TOOLKIT_PATH) {
    error(ARCGIS_TOOLKIT_PATH is not set)
}

AUGMENTED_REALITY_PATH = $$ARCGIS_TOOLKIT_PATH/augmented_reality

AR_API_PATH = $$AUGMENTED_REALITY_PATH/ArApi
AR_API_INCLUDE_PATH = $$AR_API_PATH/include
AR_API_SOURCE_PATH = $$AR_API_PATH/source
AR_API_QML_PATH = $$AR_API_PATH/qml

#-------------------------------------------------
# platform independent configuration

HEADERS += $$AR_API_INCLUDE_PATH/*.h
SOURCES += $$AR_API_SOURCE_PATH/*.cpp
RESOURCES += $$AR_API_QML_PATH/*.qrc

INCLUDEPATH += $$AR_API_INCLUDE_PATH
DEPENDPATH += $$AR_API_INCLUDE_PATH

#-------------------------------------------------
# iOS configuration

ios {
    OBJECTIVE_HEADERS += \
        $$AR_API_INCLUDE_PATH/iOS/ArKitWrapper.h

    OBJECTIVE_SOURCES += \
        $$AR_API_SOURCE_PATH/iOS/ArKitWrapper.mm \
        $$AR_API_SOURCE_PATH/iOS/ArKitUtils.mm

    HEADERS += \
        $$AR_API_INCLUDE_PATH/iOS/ArKitFrameRenderer.h \
        $$AR_API_INCLUDE_PATH/iOS/ArKitPointCloudRenderer.h \
        $$AR_API_INCLUDE_PATH/iOS/ArKitPlaneRenderer.h \
        $$AR_API_INCLUDE_PATH/iOS/ArKitUtils.h

    SOURCES += \
        $$AR_API_SOURCE_PATH/iOS/ArKitFrameRenderer.cpp \
        $$AR_API_SOURCE_PATH/iOS/ArKitPointCloudRenderer.cpp \
        $$AR_API_SOURCE_PATH/iOS/ArKitPlaneRenderer.cpp

    INCLUDEPATH += $$AR_API_INCLUDE_PATH/iOS
    DEPENDPATH += $$AR_API_INCLUDE_PATH/iOS

    LIBS += -framework ARKit
}

#-------------------------------------------------
# Android configuration

android {
    QT += core

    HEADERS += \
        $$AR_API_INCLUDE_PATH/Android/ArCoreWrapper.h \
        $$AR_API_INCLUDE_PATH/Android/ArCoreFrameRenderer.h \
        $$AR_API_INCLUDE_PATH/Android/ArCorePointCloudRenderer.h \
        $$AR_API_INCLUDE_PATH/Android/ArCorePlaneRenderer.h

    SOURCES += \
        $$AR_API_SOURCE_PATH/Android/ArCoreWrapper.cpp \
        $$AR_API_SOURCE_PATH/Android/ArCoreFrameRenderer.cpp \
        $$AR_API_SOURCE_PATH/Android/ArCorePointCloudRenderer.cpp \
        $$AR_API_SOURCE_PATH/Android/ArCorePlaneRenderer.cpp

    INCLUDEPATH += $$AR_API_INCLUDE_PATH/Android
    DEPENDPATH += $$AR_API_INCLUDE_PATH/Android

    # Extra files from the ARCore framework (Apache 2.0 licence)
    AR_CORE_PATH = $$AR_API_PATH/3rdparty/arcore
    AR_CORE_INCLUDE_PATH = $$AR_CORE_PATH/include
    AR_CORE_LIBS_PATH = $$AR_CORE_PATH/libs
    AR_CORE_GRADLE_PATH = $$AR_CORE_PATH/gradle

    HEADERS += \
        $$AR_CORE_INCLUDE_PATH/arcore_c_api.h

    INCLUDEPATH += $$AR_CORE_INCLUDE_PATH
    DEPENDPATH += $$AR_CORE_INCLUDE_PATH

    LIBS += -larcore_sdk_c

    # only arm64-v8a supported for Android
    ANDROID_ABIS = arm64-v8a
    LIBS += -L"$$AR_CORE_LIBS_PATH/jni/arm64-v8a"
    ANDROID_EXTRA_LIBS += "$$AR_CORE_LIBS_PATH/jni/arm64-v8a/libarcore_sdk_c.so"
}
