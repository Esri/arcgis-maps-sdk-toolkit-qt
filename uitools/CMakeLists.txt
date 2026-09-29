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

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(ARCGIS_RUNTIME_TOOLKIT_QML_VERSION "3.0")
set(ARCGIS_RUNTIME_TOOLKIT_CMAKE_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/ArcGISRuntimeToolkit"
)
set(ARCGIS_RUNTIME_TOOLKIT_QML_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/qml"
)

option(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK "Build the Qt Quick toolkit" ON)
if(ANDROID OR IOS)
    set(_arcgis_runtime_toolkit_widgets_default OFF)
else()
    set(_arcgis_runtime_toolkit_widgets_default ON)
endif()
option(ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS "Build the Qt Widgets toolkit" ${_arcgis_runtime_toolkit_widgets_default})

if(NOT TARGET ArcGISRuntime::Cpp)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/examples/cmake")
    find_package(ArcGISRuntime ${PROJECT_VERSION} REQUIRED COMPONENTS Cpp)
endif()

if((ANDROID OR IOS) AND ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS)
    message(FATAL_ERROR "The ArcGISRuntimeToolkit Widgets component is not supported on Android or iOS.")
endif()

if(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK)
    add_subdirectory(toolkitcpp)
endif()

if(ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS)
    add_subdirectory(toolkitwidgets)
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/ArcGISRuntimeToolkitConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ArcGISRuntimeToolkitConfig.cmake"
    INSTALL_DESTINATION "${ARCGIS_RUNTIME_TOOLKIT_CMAKE_INSTALL_DIR}"
    PATH_VARS ARCGIS_RUNTIME_TOOLKIT_QML_INSTALL_DIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ArcGISRuntimeToolkitConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ArcGISRuntimeToolkitConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ArcGISRuntimeToolkitConfigVersion.cmake"
    DESTINATION "${ARCGIS_RUNTIME_TOOLKIT_CMAKE_INSTALL_DIR}"
)

install(FILES "${CMAKE_CURRENT_LIST_DIR}/../LICENSE"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/licenses/ArcGISRuntimeToolkit"
)