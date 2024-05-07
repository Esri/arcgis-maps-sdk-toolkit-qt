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
function(setup_toolkit TARGET_NAME TOOLKIT_DIR)
    set(TOOLKITUI_DIR ${TOOLKIT_DIR}/uitools)
    set(CPPPATH ${TOOLKITUI_DIR}/cpp/Esri/ArcGISRuntime/Toolkit)
    set(REGISTERPATH ${TOOLKITUI_DIR}/register/Esri/ArcGISRuntime/Toolkit)

    file(GLOB_RECURSE TOOLKITCOMMON_HEADERS "${CPPPATH}/*.h")
    file(GLOB_RECURSE TOOLKITCOMMON_SOURCES "${CPPPATH}/*.cpp")
    file(GLOB_RECURSE TOOLKITREGISTER_HEADERS "${REGISTERPATH}/*.h")
    file(GLOB_RECURSE TOOLKITREGISTER_SOURCES "${REGISTERPATH}/*.cpp")

    set(TOOLKIT_RESOURCES
        ${TOOLKITUI_DIR}/images/esri_arcgisruntime_toolkit_images.qrc
        ${TOOLKITUI_DIR}/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc)

    target_include_directories(${TARGET_NAME} PRIVATE
        ${TOOLKITUI_DIR}/cpp
        ${TOOLKITUI_DIR}/register
        ${CPPPATH}
        ${REGISTERPATH})

    target_sources(${TARGET_NAME} PRIVATE
        ${TOOLKITCOMMON_HEADERS}
        ${TOOLKITCOMMON_SOURCES}
        ${TOOLKITREGISTER_HEADERS}
        ${TOOLKITREGISTER_SOURCES}
        ${TOOLKIT_RESOURCES})

    find_package(Qt6 COMPONENTS REQUIRED QuickControls2 WebView Svg QuickLayouts)

    target_link_libraries(${TARGET_NAME} PRIVATE
        Qt6::QuickControls2
        Qt6::WebView
        Qt6::Svg)

    if(IOS)
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::qquicklayoutsplugin)
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QuickLayouts)
    endif()

    list(APPEND QML_IMPORT_PATH ${TOOLKITUI_DIR}/import)

    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "" FORCE)

    add_definitions(-DCPP_ARCGISRUNTIME_TOOLKIT)
endfunction()
