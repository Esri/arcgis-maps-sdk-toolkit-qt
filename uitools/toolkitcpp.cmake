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
    set(TOOLKITUI_PATH ${TOOLKIT_DIR}/uitools)

    set(CPP_PATH ${TOOLKITUI_PATH}/cpp/Esri/ArcGISRuntime/Toolkit)

    set(REGISTER_PATH ${TOOLKITUI_PATH}/register/Esri/ArcGISRuntime/Toolkit)

    set(TOOLKITCOMMON_HEADERS
        ${CPP_PATH}/AuthenticationController.h
        ${CPP_PATH}/BasemapGalleryController.h
        ${CPP_PATH}/BasemapGalleryItem.h
        ${CPP_PATH}/BookmarksViewController.h
        ${CPP_PATH}/BookmarkListItem.h
        ${CPP_PATH}/CoordinateConversionConstants.h
        ${CPP_PATH}/CoordinateConversionController.h
        ${CPP_PATH}/CoordinateConversionOption.h
        ${CPP_PATH}/CoordinateConversionResult.h
        ${CPP_PATH}/CoordinateOptionDefaults.h
        ${CPP_PATH}/FloorFilterController.h
        ${CPP_PATH}/FloorFilterFacilityItem.h
        ${CPP_PATH}/FloorFilterLevelItem.h
        ${CPP_PATH}/FloorFilterSiteItem.h
        ${CPP_PATH}/LocatorSearchSource.h
        ${CPP_PATH}/Internal/BasemapGalleryImageProvider.h
        ${CPP_PATH}/Internal/GenericListModel.h
        ${CPP_PATH}/Internal/GenericTableProxyModel.h
        ${CPP_PATH}/Internal/MetaElement.h
        ${CPP_PATH}/NorthArrowController.h
        ${CPP_PATH}/OverviewMapController.h
        ${CPP_PATH}/PopupViewController.h
        ${CPP_PATH}/ScalebarController.h
        ${CPP_PATH}/SearchResult.h
        ${CPP_PATH}/SearchSourceInterface.h
        ${CPP_PATH}/SearchSuggestion.h
        ${CPP_PATH}/SearchViewController.h
        ${CPP_PATH}/SmartLocatorSearchSource.h
        ${CPP_PATH}/TimeSliderController.h
        ${CPP_PATH}/UtilityNetworkFunctionTraceResult.h
        ${CPP_PATH}/UtilityNetworkFunctionTraceResultsModel.h
        ${CPP_PATH}/UtilityNetworkListItem.h
        ${CPP_PATH}/UtilityNetworkTraceController.h
        ${CPP_PATH}/UtilityNetworkTraceStartingPoint.h
        ${CPP_PATH}/UtilityNetworkTraceStartingPointsModel.h)

    set(TOOLKITCOMMON_SOURCES
        ${CPP_PATH}/AuthenticationController.cpp
        ${CPP_PATH}/BasemapGalleryController.cpp
        ${CPP_PATH}/BasemapGalleryItem.cpp
        ${CPP_PATH}/BookmarksViewController.cpp
        ${CPP_PATH}/BookmarkListItem.cpp
        ${CPP_PATH}/CoordinateConversionConstants.cpp
        ${CPP_PATH}/CoordinateConversionController.cpp
        ${CPP_PATH}/CoordinateConversionOption.cpp
        ${CPP_PATH}/CoordinateConversionResult.cpp
        ${CPP_PATH}/CoordinateOptionDefaults.cpp
        ${CPP_PATH}/FloorFilterController.cpp
        ${CPP_PATH}/FloorFilterFacilityItem.cpp
        ${CPP_PATH}/FloorFilterLevelItem.cpp
        ${CPP_PATH}/FloorFilterSiteItem.cpp
        ${CPP_PATH}/LocatorSearchSource.cpp
        ${CPP_PATH}/Internal/BasemapGalleryImageProvider.cpp
        ${CPP_PATH}/Internal/GenericListModel.cpp
        ${CPP_PATH}/Internal/GenericTableProxyModel.cpp
        ${CPP_PATH}/Internal/MetaElement.cpp
        ${CPP_PATH}/NorthArrowController.cpp
        ${CPP_PATH}/OverviewMapController.cpp
        ${CPP_PATH}/PopupViewController.cpp
        ${CPP_PATH}/ScalebarController.cpp
        ${CPP_PATH}/SearchResult.cpp
        ${CPP_PATH}/SearchSourceInterface.cpp
        ${CPP_PATH}/SearchSuggestion.cpp
        ${CPP_PATH}/SearchViewController.cpp
        ${CPP_PATH}/SmartLocatorSearchSource.cpp
        ${CPP_PATH}/TimeSliderController.cpp
        ${CPP_PATH}/UtilityNetworkFunctionTraceResult.cpp
        ${CPP_PATH}/UtilityNetworkFunctionTraceResultsModel.cpp
        ${CPP_PATH}/UtilityNetworkListItem.cpp
        ${CPP_PATH}/UtilityNetworkTraceController.cpp
        ${CPP_PATH}/UtilityNetworkTraceStartingPoint.cpp
        ${CPP_PATH}/UtilityNetworkTraceStartingPointsModel.cpp)

    set(TOOLKITREGISTER_HEADERS
        ${REGISTER_PATH}/register.h
        ${REGISTER_PATH}/internal/register_cpp.h)

    set(TOOLKITREGISTER_SOURCES
        ${REGISTER_PATH}/register.cpp
        ${REGISTER_PATH}/internal/register_cpp.cpp)

    target_include_directories(${TARGET_NAME} PRIVATE
        ${TOOLKITUI_PATH}/cpp
        ${TOOLKITUI_PATH}/register
        ${CPP_PATH}
        ${REGISTER_PATH})

    target_sources(${TARGET_NAME} PRIVATE
        ${TOOLKITCOMMON_HEADERS}
        ${TOOLKITCOMMON_SOURCES}
        ${TOOLKITREGISTER_HEADERS}
        ${TOOLKITREGISTER_SOURCES}
        ${TOOLKITUI_PATH}/images/esri_arcgisruntime_toolkit_images.qrc
        ${TOOLKITUI_PATH}/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc)

    find_package(Qt6 COMPONENTS REQUIRED QuickControls2 WebView Svg QuickLayouts)

    target_link_libraries(${TARGET_NAME} PRIVATE
        Qt6::QuickControls2
        Qt6::WebView
        Qt6::Svg)

    list(APPEND QML_IMPORT_PATH ${TOOLKITUI_PATH}/import)

    list(REMOVE_DUPLICATES QML_IMPORT_PATH)

    set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
        CACHE STRING "QML import path for ArcGIS Runtime Toolkit"
        FORCE)

    if(IOS)
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::qquicklayoutsplugin)
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QuickLayouts)
    endif()

    target_compile_definitions(${TARGET_NAME} PRIVATE CPP_ARCGISRUNTIME_TOOLKIT)
endfunction()
