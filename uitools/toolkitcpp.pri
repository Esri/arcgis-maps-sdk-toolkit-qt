# Copyright 2012-2020 Esri
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
include($$PWD/common.pri)

QT += quickcontrols2 webview svg

REGISTERPATH = $$PWD/toolkitcpp/register/Esri/ArcGISRuntime/Toolkit

INCLUDEPATH += $$PWD/toolkitcpp/register $$REGISTERPATH

HEADERS += $$REGISTERPATH/register.h \
           $$REGISTERPATH/internal/register_cpp.h

SOURCES += $$REGISTERPATH/register.cpp \
           $$REGISTERPATH/internal/register_cpp.cpp

RESOURCES += \
  $$PWD/toolkitcpp/images/esri_arcgisruntime_toolkit_images.qrc \
  $$PWD/toolkitcpp/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc

QML_IMPORT_PATH += $$PWD/toolkitcpp/import

DEFINES += CPP_ARCGISRUNTIME_TOOLKIT MAPS_SDK_QT_TOOLKIT
