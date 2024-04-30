# Copyright 2012-2024 Esri
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
include(${CMAKE_CURRENT_LIST_DIR}/common.cmake)

set(REGISTERPATH ${CMAKE_CURRENT_LIST_DIR}/register/Esri/ArcGISRuntime/Toolkit)

set(TOOLKITREGISTER_SOURCES
    ${REGISTERPATH}/register.cpp
    ${REGISTERPATH}/internal/register_cpp.cpp)

set(TOOLKIT_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/images/esri_arcgisruntime_toolkit_images.qrc
    ${CMAKE_CURRENT_LIST_DIR}/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/register
    ${REGISTERPATH})

target_sources(${PROJECT_NAME} PRIVATE
    ${TOOLKITCOMMON_SOURCES}
    ${TOOLKITREGISTER_SOURCES}
    ${TOOLKIT_RESOURCES})

find_package(Qt6 COMPONENTS REQUIRED QuickControls2 WebView Svg QuickLayouts)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::QuickControls2
    Qt6::WebView
    Qt6::Svg)

if(IOS)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::qquicklayoutsplugin)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::QuickLayouts)
endif()

list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_LIST_DIR}/import)

set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "" FORCE)

add_definitions(-DCPP_ARCGISRUNTIME_TOOLKIT)
