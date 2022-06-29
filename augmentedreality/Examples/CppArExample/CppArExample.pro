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

TEMPLATE = app

QT += core gui opengl network positioning sensors qml quick multimedia
CONFIG += c++14

TARGET = CppArExample

ARCGIS_RUNTIME_VERSION = 100.15.0
include($$PWD/arcgisruntime.pri)

HEADERS += \
    AppInfo.h \
    CppArExample.h

SOURCES += \
    main.cpp \
    CppArExample.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

OTHER_FILES += \
    qml/*.qml \
    qml/*.js

#-------------------------------------------------------------------------------
# AR configuration

# The path to the ArcGIS runtime toolkit for Qt sources, corresponding to the files downloaded
# from the GitHub repo: https://github.com/Esri/arcgis-runtime-toolkit-qt

ARCGIS_TOOLKIT_PATH = # must be set to the root path of the toolkit folder

isEmpty(ARCGIS_TOOLKIT_PATH) {
    error(ARCGIS_TOOLKIT_PATH is not set)
}

include($$ARCGIS_TOOLKIT_PATH/augmentedreality/CppApi/ArCppApi.pri)

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
