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

REGISTERPATH = $$PWD/register/Esri/ArcGISRuntime/Toolkit

INCLUDEPATH += $$PWD/register $$REGISTERPATH

HEADERS += $$REGISTERPATH/register.h \
           $$REGISTERPATH/internal/register_qml.h

SOURCES += $$REGISTERPATH/register.cpp \
           $$REGISTERPATH/internal/register_qml.cpp

RESOURCES += \
  $$PWD/images/esri_arcgisruntime_toolkit_images.qrc \
  $$PWD/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc \
  $$PWD/import/Esri/ArcGISRuntime/Toolkit/Controller/esri_arcgisruntime_toolkit_controller.qrc

QML_IMPORT_PATH += $$PWD/import

DEFINES += QML_ARCGISRUNTIME_TOOLKIT
