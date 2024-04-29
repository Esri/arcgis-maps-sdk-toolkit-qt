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
set(CPPPATH ${CMAKE_CURRENT_LIST_DIR}/cpp/Esri/ArcGISRuntime/Toolkit)

set(REGISTERPATH ${CMAKE_CURRENT_LIST_DIR}/register/Esri/ArcGISRuntime/Toolkit)

set(TOOLKITCPP_SOURCES
    ${CPPPATH}/AuthenticationController.cpp
    ${CPPPATH}/BasemapGalleryController.cpp
    ${CPPPATH}/BasemapGalleryItem.cpp
    ${CPPPATH}/BookmarksViewController.cpp
    ${CPPPATH}/BookmarkListItem.cpp
    ${CPPPATH}/CoordinateConversionConstants.cpp
    ${CPPPATH}/CoordinateConversionController.cpp
    ${CPPPATH}/CoordinateConversionOption.cpp
    ${CPPPATH}/CoordinateConversionResult.cpp
    ${CPPPATH}/CoordinateOptionDefaults.cpp
    ${CPPPATH}/FloorFilterController.cpp
    ${CPPPATH}/FloorFilterFacilityItem.cpp
    ${CPPPATH}/FloorFilterLevelItem.cpp
    ${CPPPATH}/FloorFilterSiteItem.cpp
    ${CPPPATH}/LocatorSearchSource.cpp
    ${CPPPATH}/Internal/BasemapGalleryImageProvider.cpp
    ${CPPPATH}/Internal/GenericListModel.cpp
    ${CPPPATH}/Internal/GenericTableProxyModel.cpp
    ${CPPPATH}/Internal/MetaElement.cpp
    ${CPPPATH}/NorthArrowController.cpp
    ${CPPPATH}/OverviewMapController.cpp
    ${CPPPATH}/PopupViewController.cpp
    ${CPPPATH}/ScalebarController.cpp
    ${CPPPATH}/SearchResult.cpp
    ${CPPPATH}/SearchSourceInterface.cpp
    ${CPPPATH}/SearchSuggestion.cpp
    ${CPPPATH}/SearchViewController.cpp
    ${CPPPATH}/SmartLocatorSearchSource.cpp
    ${CPPPATH}/TimeSliderController.cpp
    ${CPPPATH}/UtilityNetworkFunctionTraceResult.cpp
    ${CPPPATH}/UtilityNetworkFunctionTraceResultsModel.cpp
    ${CPPPATH}/UtilityNetworkListItem.cpp
    ${CPPPATH}/UtilityNetworkTraceController.cpp
    ${CPPPATH}/UtilityNetworkTraceStartingPoint.cpp
    ${CPPPATH}/UtilityNetworkTraceStartingPointsModel.cpp
    ${REGISTERPATH}/register.cpp
    ${REGISTERPATH}/internal/register_cpp.cpp)

set(TOOLKITCPP_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/images/esri_arcgisruntime_toolkit_images.qrc
    ${CMAKE_CURRENT_LIST_DIR}/import/Esri/ArcGISRuntime/Toolkit/esri_arcgisruntime_toolkit_view.qrc)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpp
    ${CPPPATH}
    ${CMAKE_CURRENT_LIST_DIR}/register
    ${REGISTERPATH})

target_sources(${PROJECT_NAME} PRIVATE
    ${TOOLKITCPP_SOURCES}
    ${TOOLKITCPP_RESOURCES})

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

add_definitions(-DCPP_ARCGISRUNTIME_TOOLKIT)
