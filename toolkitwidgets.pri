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

RESOURCES += $$PWD/images/esri_arcgisruntime_toolkit_images.qrc

DEFINES += WIDGETS_ARCGISRUNTIME_TOOLKIT

INCLUDEPATH += $$PWD/widgets

WIDGETPATH = $$PWD/widgets/Esri/ArcGISRuntime/Toolkit

HEADERS += $$WIDGETPATH/CoordinateEditDelegate.h \
           $$WIDGETPATH/CoordinateConversion.h \
           $$WIDGETPATH/Flash.h \
           $$WIDGETPATH/NorthArrow.h

SOURCES += $$WIDGETPATH/CoordinateEditDelegate.cpp \ 
           $$WIDGETPATH/CoordinateConversion.cpp \
           $$WIDGETPATH/Flash.cpp \
           $$WIDGETPATH/NorthArrow.cpp

FORMS += $$WIDGETPATH/CoordinateConversion.ui
