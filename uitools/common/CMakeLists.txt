# Copyright 2012-2025 Esri
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
cmake_minimum_required(VERSION 3.10)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_SOURCES
    # public sources
    ../common/src/ArcGISAuthenticationController.cpp
    ../common/src/AuthenticationController.cpp
    ../common/src/BasemapGalleryController.cpp
    ../common/src/BasemapGalleryItem.cpp
    ../common/src/BookmarkListItem.cpp
    ../common/src/BookmarksViewController.cpp
    ../common/src/CoordinateConversionConstants.cpp
    ../common/src/CoordinateConversionController.cpp
    ../common/src/CoordinateConversionOption.cpp
    ../common/src/CoordinateConversionResult.cpp
    ../common/src/CoordinateOptionDefaults.cpp
    ../common/src/FloorFilterController.cpp
    ../common/src/FloorFilterFacilityItem.cpp
    ../common/src/FloorFilterLevelItem.cpp
    ../common/src/FloorFilterSiteItem.cpp
    ../common/src/LocatorSearchSource.cpp
    ../common/src/NorthArrowController.cpp
    ../common/src/OverviewMapController.cpp
    ../common/src/PopupViewController.cpp
    ../common/src/ScalebarController.cpp
    ../common/src/SearchResult.cpp
    ../common/src/SearchSourceInterface.cpp
    ../common/src/SearchSuggestion.cpp
    ../common/src/SearchViewController.cpp
    ../common/src/SmartLocatorSearchSource.cpp
    ../common/src/TimeSliderController.cpp
    ../common/src/UtilityNetworkFunctionTraceResult.cpp
    ../common/src/UtilityNetworkFunctionTraceResultsModel.cpp
    ../common/src/UtilityNetworkTraceController.cpp
    ../common/src/UtilityNetworkTraceStartingPoint.cpp
    ../common/src/UtilityNetworkTraceStartingPointsModel.cpp

    # internal sources
    ../common/src/Internal/ArcGISAuthenticationChallengeRelay.cpp
    ../common/src/Internal/BasemapGalleryImageProvider.cpp
    ../common/src/Internal/GenericListModel.cpp
    ../common/src/Internal/GenericTableProxyModel.cpp
    ../common/src/Internal/MetaElement.cpp
    ../common/src/Internal/NetworkAuthenticationChallengeRelay.cpp
)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_HEADERS
    #public headers
    ../common/src/ArcGISAuthenticationController.h
    ../common/src/AuthenticationController.h
    ../common/src/BasemapGalleryController.h
    ../common/src/BasemapGalleryItem.h
    ../common/src/BookmarkListItem.h
    ../common/src/BookmarksViewController.h
    ../common/src/CoordinateConversionConstants.h
    ../common/src/CoordinateConversionController.h
    ../common/src/CoordinateConversionOption.h
    ../common/src/CoordinateConversionResult.h
    ../common/src/CoordinateOptionDefaults.h
    ../common/src/FloorFilterController.h
    ../common/src/FloorFilterFacilityItem.h
    ../common/src/FloorFilterLevelItem.h
    ../common/src/FloorFilterSiteItem.h
    ../common/src/LocatorSearchSource.h
    ../common/src/NorthArrowController.h
    ../common/src/OverviewMapController.h
    ../common/src/PopupViewController.h
    ../common/src/ScalebarController.h
    ../common/src/SearchResult.h
    ../common/src/SearchSourceInterface.h
    ../common/src/SearchSuggestion.h
    ../common/src/SearchViewController.h
    ../common/src/SmartLocatorSearchSource.h
    ../common/src/TimeSliderController.h
    ../common/src/UtilityNetworkFunctionTraceResult.h
    ../common/src/UtilityNetworkFunctionTraceResultsModel.h
    ../common/src/UtilityNetworkTraceController.h
    ../common/src/UtilityNetworkTraceStartingPoint.h
    ../common/src/UtilityNetworkTraceStartingPointsModel.h

    # internal headers
    ../common/src/Internal/ArcGISAuthenticationChallengeRelay.h
    ../common/src/Internal/BasemapGalleryImageProvider.h
    ../common/src/Internal/DisconnectOnSignal.h
    ../common/src/Internal/DoOnLoad.h
    ../common/src/Internal/GenericListModel.h
    ../common/src/Internal/GenericTableProxyModel.h
    ../common/src/Internal/GeoViews.h
    ../common/src/Internal/MetaElement.h
    ../common/src/Internal/NetworkAuthenticationChallengeRelay.h
    ../common/src/Internal/SingleShotConnection.h
)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_INCLUDES
    ../common/src
    ../common/src/Internal
)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES
    ../common/images/basemap.svg
    ../common/images/chevron-down.svg
    ../common/images/chevron-left.svg
    ../common/images/chevron-up.svg
    ../common/images/chevrons-left.svg
    ../common/images/chevrons-right.svg
    ../common/images/compass.svg
    ../common/images/cursor-click.svg
    ../common/images/edit-attributes.svg
    ../common/images/ellipsis.svg
    ../common/images/flash.svg
    ../common/images/forward.svg
    ../common/images/information.svg
    ../common/images/layer-zoom-to.svg
    ../common/images/organization.svg
    ../common/images/pause.svg
    ../common/images/pin-tear.png
    ../common/images/pin-tear.svg
    ../common/images/pin.svg
    ../common/images/play.svg
    ../common/images/plus-circle.svg
    ../common/images/reverse.svg
    ../common/images/search.svg
    ../common/images/trash.svg
    ../common/images/x.svg
    ../common/images/zoom-to-object.svg
)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_BASE
    "../common/images/"
)

set(ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_PREFIX
    "/Esri/ArcGISRuntime/Toolkit/"
)
