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

mac {
    cache()
}

#-------------------------------------------------------------------------------

CONFIG += c++11

QT += core gui opengl network positioning sensors qml quick

ARCGIS_RUNTIME_VERSION = 100.10
include($$PWD/arcgisruntime.pri)

TEMPLATE = app
TARGET = QmlArExample

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.1")
    }
    equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 1) {
        error("$$TARGET requires Qt 5.15.1")
    }
}

#-------------------------------------------------------------------------------

HEADERS += \
    AppInfo.h

SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

OTHER_FILES += \
    wizard.xml \
    wizard.png \
    qml/*.qml \
    qml/scenes/*.qml \
    qml/*.js

#-------------------------------------------------------------------------------
# AR configuration

# The path to the ArcGIS runtime toolkit for Qt sources, corresponding to the files downloaded
# from the GitHub repo: https://github.com/Esri/arcgis-runtime-toolkit-qt

ARCGIS_TOOLKIT_PATH = # must be set to the root path of the toolkit folder

isEmpty(ARCGIS_TOOLKIT_PATH) {
    error(ARCGIS_TOOLKIT_PATH is not set)
}

include($$ARCGIS_TOOLKIT_PATH/AugmentedReality/QmlApi/ArQmlApi.pri)

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

android: include(/Users/jame5907/Library/Android/sdk/android_openssl/openssl.pri)

ANDROID_EXTRA_LIBS = /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libruntimecore_arm64-v8a.so /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libEsriCommonQt_arm64-v8a.so /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libruntimecore_armeabi-v7a.so /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libEsriCommonQt_armeabi-v7a.so /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libruntimecore_x86.so /Users/jame5907/ArcGIS_SDKs/Qt100.10/sdk/android/lib/libEsriCommonQt_x86.so /Users/jame5907/applications/qt/sdk/toolkit/AugmentedReality/Common/3rdparty/arcore/libs/jni/arm64-v8a/libarcore_sdk_c.so /Users/jame5907/applications/qt/sdk/toolkit/AugmentedReality/Common/3rdparty/arcore/libs/jni/arm64-v8a/libarcore_sdk_jni.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/arm/libcrypto_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/arm/libssl_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/arm64/libcrypto_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/arm64/libssl_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/x86/libcrypto_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/x86/libssl_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/x86_64/libcrypto_1_1.so /Users/jame5907/Library/Android/sdk/android_openssl/no-asm/latest/x86_64/libssl_1_1.so
