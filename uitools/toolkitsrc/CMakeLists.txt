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
cmake_minimum_required(VERSION 3.5)

# Extract the name of the parent directory
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Check if PARENT_DIR contains "Widget"
set(FOUND_INDEX -1)
string(FIND "${PARENT_DIR}" "Widget" FOUND_INDEX)

# Set IS_WIDGET to false by default
set(IS_WIDGET false)

# Set IS_WIDGET to true if "Widget" is found in PARENT_DIR
if(${FOUND_INDEX} GREATER -1)
# Set the project name to "lib" + parent directory name
set(PROJECT_LIB "lib${PARENT_DIR}")

# Use the variable in the project command
project(${PROJECT_LIB} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Use the variable in the project command
project(${PROJECT_LIB} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(IS_WIDGET)
    set(CMAKE_AUTOUIC ON)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Svg WebEngineWidgets Widgets)
else()
    find_package(Qt6 REQUIRED COMPONENTS Core Gui QuickControls2 Svg WebView)
endif()

qt_standard_project_setup()

qt_add_library(${PROJECT_LIB} STATIC
    cpp/Esri/ArcGISRuntime/Toolkit/AuthenticationController.cpp cpp/Esri/ArcGISRuntime/Toolkit/AuthenticationController.h
    cpp/Esri/ArcGISRuntime/Toolkit/BasemapGalleryController.cpp cpp/Esri/ArcGISRuntime/Toolkit/BasemapGalleryController.h
    cpp/Esri/ArcGISRuntime/Toolkit/BasemapGalleryItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/BasemapGalleryItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/BookmarkListItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/BookmarkListItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/BookmarksViewController.cpp cpp/Esri/ArcGISRuntime/Toolkit/BookmarksViewController.h
    cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionConstants.cpp cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionConstants.h
    cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionController.cpp cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionController.h
    cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionOption.cpp cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionOption.h
    cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionResult.cpp cpp/Esri/ArcGISRuntime/Toolkit/CoordinateConversionResult.h
    cpp/Esri/ArcGISRuntime/Toolkit/CoordinateOptionDefaults.cpp cpp/Esri/ArcGISRuntime/Toolkit/CoordinateOptionDefaults.h
    cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterController.cpp cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterController.h
    cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterFacilityItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterFacilityItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterLevelItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterLevelItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterSiteItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/FloorFilterSiteItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/BasemapGalleryImageProvider.cpp cpp/Esri/ArcGISRuntime/Toolkit/Internal/BasemapGalleryImageProvider.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/DisconnectOnSignal.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/DoOnLoad.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/GenericListModel.cpp cpp/Esri/ArcGISRuntime/Toolkit/Internal/GenericListModel.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/GenericTableProxyModel.cpp cpp/Esri/ArcGISRuntime/Toolkit/Internal/GenericTableProxyModel.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/GeoViews.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/MetaElement.cpp cpp/Esri/ArcGISRuntime/Toolkit/Internal/MetaElement.h
    cpp/Esri/ArcGISRuntime/Toolkit/Internal/SingleShotConnection.h
    cpp/Esri/ArcGISRuntime/Toolkit/LocatorSearchSource.cpp cpp/Esri/ArcGISRuntime/Toolkit/LocatorSearchSource.h
    cpp/Esri/ArcGISRuntime/Toolkit/NorthArrowController.cpp cpp/Esri/ArcGISRuntime/Toolkit/NorthArrowController.h
    cpp/Esri/ArcGISRuntime/Toolkit/OverviewMapController.cpp cpp/Esri/ArcGISRuntime/Toolkit/OverviewMapController.h
    cpp/Esri/ArcGISRuntime/Toolkit/PopupViewController.cpp cpp/Esri/ArcGISRuntime/Toolkit/PopupViewController.h
    cpp/Esri/ArcGISRuntime/Toolkit/ScalebarController.cpp cpp/Esri/ArcGISRuntime/Toolkit/ScalebarController.h
    cpp/Esri/ArcGISRuntime/Toolkit/SearchResult.cpp cpp/Esri/ArcGISRuntime/Toolkit/SearchResult.h
    cpp/Esri/ArcGISRuntime/Toolkit/SearchSourceInterface.cpp cpp/Esri/ArcGISRuntime/Toolkit/SearchSourceInterface.h
    cpp/Esri/ArcGISRuntime/Toolkit/SearchSuggestion.cpp cpp/Esri/ArcGISRuntime/Toolkit/SearchSuggestion.h
    cpp/Esri/ArcGISRuntime/Toolkit/SearchViewController.cpp cpp/Esri/ArcGISRuntime/Toolkit/SearchViewController.h
    cpp/Esri/ArcGISRuntime/Toolkit/SmartLocatorSearchSource.cpp cpp/Esri/ArcGISRuntime/Toolkit/SmartLocatorSearchSource.h
    cpp/Esri/ArcGISRuntime/Toolkit/TimeSliderController.cpp cpp/Esri/ArcGISRuntime/Toolkit/TimeSliderController.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkFunctionTraceResult.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkFunctionTraceResult.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkFunctionTraceResultsModel.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkFunctionTraceResultsModel.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkListItem.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkListItem.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceController.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceController.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceStartingPoint.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceStartingPoint.h
    cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceStartingPointsModel.cpp cpp/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTraceStartingPointsModel.h
    register/Esri/ArcGISRuntime/Toolkit/internal/register_cpp.cpp register/Esri/ArcGISRuntime/Toolkit/internal/register_cpp.h
    register/Esri/ArcGISRuntime/Toolkit/register.cpp register/Esri/ArcGISRuntime/Toolkit/register.h
)
# Add widget-related source files if IS_WIDGET is true
if(IS_WIDGET)
    target_sources(${PROJECT_LIB} PRIVATE
        widgets/Esri/ArcGISRuntime/Toolkit/AuthenticationView.cpp widgets/Esri/ArcGISRuntime/Toolkit/AuthenticationView.h widgets/Esri/ArcGISRuntime/Toolkit/AuthenticationView.ui
        widgets/Esri/ArcGISRuntime/Toolkit/BasemapGallery.cpp widgets/Esri/ArcGISRuntime/Toolkit/BasemapGallery.h widgets/Esri/ArcGISRuntime/Toolkit/BasemapGallery.ui
        widgets/Esri/ArcGISRuntime/Toolkit/BookmarksView.cpp widgets/Esri/ArcGISRuntime/Toolkit/BookmarksView.h widgets/Esri/ArcGISRuntime/Toolkit/BookmarksView.ui
        widgets/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.cpp widgets/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.h widgets/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.ui
        widgets/Esri/ArcGISRuntime/Toolkit/FloorFilter.cpp widgets/Esri/ArcGISRuntime/Toolkit/FloorFilter.h widgets/Esri/ArcGISRuntime/Toolkit/FloorFilter.ui
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.h widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.ui
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.h widgets/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.ui
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/CoordinateEditDelegate.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/CoordinateEditDelegate.h
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/Flash.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/Flash.h
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.h widgets/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.ui
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.h widgets/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.ui
        widgets/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.cpp widgets/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.h widgets/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.ui
        widgets/Esri/ArcGISRuntime/Toolkit/NorthArrow.cpp widgets/Esri/ArcGISRuntime/Toolkit/NorthArrow.h
        widgets/Esri/ArcGISRuntime/Toolkit/OverviewMap.cpp widgets/Esri/ArcGISRuntime/Toolkit/OverviewMap.h widgets/Esri/ArcGISRuntime/Toolkit/OverviewMap.ui
    )
endif()

target_include_directories(${PROJECT_LIB} PUBLIC
    cpp
    cpp/Esri/ArcGISRuntime/Toolkit
)

if(IS_WIDGET)
    target_include_directories(${PROJECT_LIB} PUBLIC
        widgets
        widgets/Esri/ArcGISRuntime/Toolkit
    )
else()
    target_include_directories(${PROJECT_LIB} PUBLIC
        register
        register/Esri/ArcGISRuntime/Toolkit
    )
endif()

target_compile_definitions(${PROJECT_LIB} PRIVATE)

if(IS_WIDGET)
    target_compile_definitions(${PROJECT_LIB} PRIVATE
        WIDGETS_ARCGISRUNTIME_TOOLKIT
    )
else()
    target_compile_definitions(${PROJECT_LIB} PRIVATE
        CPP_ARCGISRUNTIME_TOOLKIT
        MAPS_SDK_QT_TOOLKIT
    )
endif()

target_link_libraries(${PROJECT_LIB} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Svg
    ArcGISRuntime::Cpp
)

if(IS_WIDGET)
    target_link_libraries(${PROJECT_LIB} PRIVATE
        Qt6::WebEngineWidgets
        Qt6::Widgets
    )
else()
    target_link_libraries(${PROJECT_LIB} PRIVATE
        Qt6::QuickControls2
        Qt6::WebView
    )
endif()

set(esri_arcgisruntime_toolkit_images_resource_files
    "images/basemap.svg"
    "images/chevron-down.svg"
    "images/chevron-left.svg"
    "images/chevron-up.svg"
    "images/chevrons-left.svg"
    "images/chevrons-right.svg"
    "images/compass.svg"
    "images/cursor-click.svg"
    "images/edit-attributes.svg"
    "images/ellipsis.svg"
    "images/flash.svg"
    "images/forward.svg"
    "images/information.svg"
    "images/layer-zoom-to.svg"
    "images/organization.svg"
    "images/pause.svg"
    "images/pin-tear.svg"
    "images/pin-tear.png"
    "images/pin.svg"
    "images/play.svg"
    "images/plus-circle.svg"
    "images/reverse.svg"
    "images/search.svg"
    "images/trash.svg"
    "images/x.svg"
    "images/zoom-to-object.svg"
)

qt_add_resources(${PROJECT_LIB}  "esri_arcgisruntime_toolkit_images"
    PREFIX
        "/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images"
    BASE
        "images"
    FILES
        ${esri_arcgisruntime_toolkit_images_resource_files}
)

if(NOT IS_WIDGET)
    set(esri_arcgisruntime_toolkit_view_resource_files
        "import/Esri/ArcGISRuntime/Toolkit/AuthenticationView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/BasemapGallery.qml"
        "import/Esri/ArcGISRuntime/Toolkit/BookmarksView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/Callout.qml"
        "import/Esri/ArcGISRuntime/Toolkit/ClientCertificateView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.qml"
        "import/Esri/ArcGISRuntime/Toolkit/CurrentVersion.qml"
        "import/Esri/ArcGISRuntime/Toolkit/FlashImage.qml"
        "import/Esri/ArcGISRuntime/Toolkit/FloorFilter.qml"
        "import/Esri/ArcGISRuntime/Toolkit/NorthArrow.qml"
        "import/Esri/ArcGISRuntime/Toolkit/OAuth2View.qml"
        "import/Esri/ArcGISRuntime/Toolkit/OverviewMap.qml"
        "import/Esri/ArcGISRuntime/Toolkit/PopupStackView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/PopupView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/Scalebar.qml"
        "import/Esri/ArcGISRuntime/Toolkit/ScalebarImpl.qml"
        "import/Esri/ArcGISRuntime/Toolkit/SearchView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/SslHandshakeView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/TimeSlider.qml"
        "import/Esri/ArcGISRuntime/Toolkit/UserCredentialsView.qml"
        "import/Esri/ArcGISRuntime/Toolkit/UtilityNetworkTrace.qml"
        "import/Esri/ArcGISRuntime/Toolkit/qmldir"
    )

    qt_add_resources(${PROJECT_LIB}  "esri_arcgisruntime_toolkit_view"
        PREFIX
            "/esri.com/imports/Esri/ArcGISRuntime/Toolkit"
        BASE
            "import/Esri/ArcGISRuntime/Toolkit"
        FILES
            ${esri_arcgisruntime_toolkit_view_resource_files}
    )

    list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_LIST_DIR}/import)

    list(REMOVE_DUPLICATES QML_IMPORT_PATH)

    set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
        CACHE STRING "ArcGISRuntime toolkit qml import paths"
        FORCE
    )
endif()
