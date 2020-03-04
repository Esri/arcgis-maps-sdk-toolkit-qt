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
INCLUDEPATH += $$PWD/cpp

CPPPATH = $$PWD/cpp/Esri/ArcGISRuntime/Toolkit

HEADERS += $$CPPPATH/CoordinateConversionConstants.h \
           $$CPPPATH/CoordinateConversionController.h \
           $$CPPPATH/CoordinateConversionOption.h \
           $$CPPPATH/CoordinateConversionResult.h \
           $$CPPPATH/CoordinateOptionDefaults.h \
           $$CPPPATH/GenericListModel.h \
           $$CPPPATH/GenericTableProxyModel.h \
           $$CPPPATH/GeoViews.h \
           $$CPPPATH/MetaElement.h \
           $$CPPPATH/NorthArrowController.h

SOURCES += $$CPPPATH/CoordinateConversionConstants.cpp \
           $$CPPPATH/CoordinateConversionController.cpp \
           $$CPPPATH/CoordinateConversionOption.cpp \
           $$CPPPATH/CoordinateConversionResult.cpp \
           $$CPPPATH/CoordinateOptionDefaults.cpp \
           $$CPPPATH/GenericListModel.cpp \
           $$CPPPATH/GenericTableProxyModel.cpp \
           $$CPPPATH/MetaElement.cpp \
           $$CPPPATH/NorthArrowController.cpp