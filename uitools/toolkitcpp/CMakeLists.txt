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
project(libtoolkitcpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui QuickControls2 Svg WebView)

qt_standard_project_setup()

qt_add_library(libtoolkitcpp STATIC
    cpp/Esri/ArcGISRuntime/Toolkit/ArcGISAuthenticationController.cpp cpp/Esri/ArcGISRuntime/Toolkit/ArcGISAuthenticationController.h
    cpp/Esri/ArcGISRuntime/Toolkit/ArcGISAuthenticationChallengeRelay.cpp cpp/Esri/ArcGISRuntime/Toolkit/ArcGISAuthenticationChallengeRelay.h
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
    cpp/Esri/ArcGISRuntime/Toolkit/NetworkAuthenticationChallengeRelay.cpp cpp/Esri/ArcGISRuntime/Toolkit/NetworkAuthenticationChallengeRelay.h
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

target_include_directories(libtoolkitcpp PUBLIC
    cpp
    cpp/Esri/ArcGISRuntime/Toolkit
    register
    register/Esri/ArcGISRuntime/Toolkit
)

target_compile_definitions(libtoolkitcpp PRIVATE
    CPP_ARCGISRUNTIME_TOOLKIT
    MAPS_SDK_QT_TOOLKIT
)

target_link_libraries(libtoolkitcpp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::WebView
    ArcGISRuntime::Cpp
)

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

qt_add_resources(libtoolkitcpp "esri_arcgisruntime_toolkit_images"
    PREFIX
        "/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images"
    BASE
        "images"
    FILES
        ${esri_arcgisruntime_toolkit_images_resource_files}
)

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

qt_add_resources(libtoolkitcpp "esri_arcgisruntime_toolkit_view"
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
