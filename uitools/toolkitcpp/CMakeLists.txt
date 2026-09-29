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
cmake_minimum_required(VERSION 3.16)
project(libtoolkitcpp VERSION 1.0 LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/../common/CMakeLists.txt")

include(GNUInstallDirs)

if(NOT DEFINED ARCGIS_RUNTIME_TOOLKIT_QML_VERSION)
    set(ARCGIS_RUNTIME_TOOLKIT_QML_VERSION "3.0")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui QuickControls2 Svg WebView Graphs NetworkAuth)

qt_standard_project_setup()

qt_add_library(libtoolkitcpp STATIC
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_HEADERS}
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_SOURCES}
    src/Esri/ArcGISRuntime/Toolkit/register.h
    src/Esri/ArcGISRuntime/Toolkit/register.cpp
)

add_library(ArcGISRuntimeToolkit::Quick ALIAS libtoolkitcpp)
set_target_properties(libtoolkitcpp PROPERTIES EXPORT_NAME Quick)
target_compile_features(libtoolkitcpp PUBLIC cxx_std_17)

target_include_directories(libtoolkitcpp PUBLIC
    $<BUILD_INTERFACE:${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_INCLUDES}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esri/ArcGISRuntime/Toolkit>
)

target_compile_definitions(libtoolkitcpp PRIVATE
    CPP_ARCGISRUNTIME_TOOLKIT
)

target_link_libraries(libtoolkitcpp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Graphs
    Qt6::NetworkAuth
    ArcGISRuntime::Cpp
)

target_link_libraries(libtoolkitcpp PUBLIC
    Qt6::WebView
)

if(IOS)
    target_link_libraries(libtoolkitcpp PRIVATE
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IOS_FRAMEWORKS}
    )
endif()

set(esri_arcgisruntime_toolkit_view_resource_files
    "import/Esri/ArcGISRuntime/Toolkit/AttachmentsPopupElementView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/Authenticator.qml"
    "import/Esri/ArcGISRuntime/Toolkit/AuthenticatorClientCertificateView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/AuthenticatorOAuth2View.qml"
    "import/Esri/ArcGISRuntime/Toolkit/AuthenticatorServerTrustView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/AuthenticatorUserCredentialsView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/BasemapGallery.qml"
    "import/Esri/ArcGISRuntime/Toolkit/BookmarksView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/Callout.qml"
    "import/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.qml"
    "import/Esri/ArcGISRuntime/Toolkit/CurrentVersion.qml"
    "import/Esri/ArcGISRuntime/Toolkit/FieldsPopupElementView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/FlashImage.qml"
    "import/Esri/ArcGISRuntime/Toolkit/FloorFilter.qml"
    "import/Esri/ArcGISRuntime/Toolkit/MediaPopupElementView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/NorthArrow.qml"
    "import/Esri/ArcGISRuntime/Toolkit/OverviewMap.qml"
    "import/Esri/ArcGISRuntime/Toolkit/PopupView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/Scalebar.qml"
    "import/Esri/ArcGISRuntime/Toolkit/ScalebarImpl.qml"
    "import/Esri/ArcGISRuntime/Toolkit/SearchView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/TextPopupElementView.qml"
    "import/Esri/ArcGISRuntime/Toolkit/TimeSlider.qml"
    "import/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTrace.qml"
    "import/Esri/ArcGISRuntime/Toolkit/qmldir"
)

qt_add_qml_module(libtoolkitcpp
    URI "Esri.ArcGISRuntime.Toolkit"
    VERSION "${ARCGIS_RUNTIME_TOOLKIT_QML_VERSION}"
    RESOURCE_PREFIX "/esri.com/imports"
    OUTPUT_TARGETS toolkitcpp_qml_output_targets
    QML_FILES
        ${esri_arcgisruntime_toolkit_view_resource_files}
    NO_PLUGIN
)

list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_LIST_DIR}/import)

list(REMOVE_DUPLICATES QML_IMPORT_PATH)

set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "ArcGISRuntime toolkit qml import paths"
    FORCE
)

qt_add_resources(libtoolkitcpp "ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES"
    PREFIX
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_PREFIX}
    BASE
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_BASE}
    OUTPUT_TARGETS toolkitcpp_common_resource_targets
    FILES
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES}
)

install(TARGETS
    libtoolkitcpp
    ${toolkitcpp_qml_output_targets}
    ${toolkitcpp_common_resource_targets}
    EXPORT ArcGISRuntimeToolkitQuickTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}/ArcGISRuntimeToolkit/objects
)

install(FILES
    src/Esri/ArcGISRuntime/Toolkit/register.h
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esri/ArcGISRuntime/Toolkit
)

install(EXPORT ArcGISRuntimeToolkitQuickTargets
    FILE ArcGISRuntimeToolkitQuickTargets.cmake
    NAMESPACE ArcGISRuntimeToolkit::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ArcGISRuntimeToolkit
)

install(DIRECTORY
    "${CMAKE_CURRENT_LIST_DIR}/import/Esri/ArcGISRuntime/Toolkit/"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/qml/Esri/ArcGISRuntime/Toolkit
    FILES_MATCHING
        PATTERN "*.qml"
        PATTERN "qmldir"
)
