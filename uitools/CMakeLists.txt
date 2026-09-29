# Copyright 2012-2026 Esri
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
cmake_minimum_required(VERSION 3.16)

project(ArcGISRuntimeToolkit VERSION 300.2.0 LANGUAGES CXX)

option(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK "Build the Qt Quick toolkit" OFF)
option(ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS "Build the Qt Widgets toolkit" OFF)

if(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK AND ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS)
    message(FATAL_ERROR "Select only one ArcGISRuntimeToolkit UI implementation: Quick or Widgets.")
elseif(NOT ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK AND NOT ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS)
    message(FATAL_ERROR "Select an ArcGISRuntimeToolkit UI implementation by enabling Quick or Widgets.")
endif()

if((ANDROID OR IOS) AND ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS)
    message(FATAL_ERROR "The ArcGISRuntimeToolkit Widgets component is not supported on Android or iOS.")
endif()

if(NOT TARGET ArcGISRuntime::Cpp)
    message(FATAL_ERROR
        "ArcGISRuntime::Cpp must exist before adding ArcGISRuntimeToolkit. "
        "Load the ArcGIS Maps SDK for Qt CMake integration first."
    )
endif()

if(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK)
    add_subdirectory(toolkitcpp)
else()
    add_subdirectory(toolkitwidgets)
endif()