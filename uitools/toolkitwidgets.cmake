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
function(setup_tookitwidgets TARGET_NAME TOOLKIT_DIR)
    set(TOOLKITUI_DIR ${TOOLKIT_DIR}/uitools)
    set(CPPPATH ${TOOLKITUI_DIR}/cpp/Esri/ArcGISRuntime/Toolkit)
    set(WIDGETPATH ${TOOLKITUI_DIR}/widgets/Esri/ArcGISRuntime/Toolkit)

    message(TOOLKITUI_DIR ${TOOLKITUI_DIR})

    file(GLOB_RECURSE TOOLKITCOMMON_HEADERS "${CPPPATH}/*.h")
    file(GLOB_RECURSE TOOLKITCOMMON_SOURCES "${CPPPATH}/*.cpp")
    file(GLOB_RECURSE TOOLKITWIDGET_HEADERS "${WIDGETPATH}/*.h")
    file(GLOB_RECURSE TOOLKITWIDGET_SOURCES "${WIDGETPATH}/*.cpp")
    file(GLOB_RECURSE TOOLKITWIDGET_FORMS "${WIDGETPATH}/*.ui")

    set(TOOLKITWIDGET_RESOURCES ${TOOLKITUI_DIR}/images/esri_arcgisruntime_toolkit_images.qrc)

    target_include_directories(${TARGET_NAME} PRIVATE
        ${TOOLKITUI_DIR}/cpp
        ${TOOLKITUI_DIR}/widgets
        ${CPPPATH}
        ${WIDGETPATH})

    target_sources(${TARGET_NAME} PRIVATE
        ${TOOLKITCOMMON_HEADERS}
        ${TOOLKITCOMMON_SOURCES}
        ${TOOLKITWIDGET_HEADERS}
        ${TOOLKITWIDGET_SOURCES}
        ${TOOLKITWIDGET_RESOURCES}
        ${TOOLKITWIDGET_FORMS})

    find_package(Qt6 COMPONENTS REQUIRED Widgets WebEngineWidgets Svg)

    target_link_libraries(${TARGET_NAME} PRIVATE
        Qt::Widgets
        Qt::WebEngineWidgets
        Qt::Svg)

    add_definitions(-DWIDGETS_ARCGISRUNTIME_TOOLKIT)
endfunction()



