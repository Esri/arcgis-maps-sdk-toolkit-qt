# Copyright 2012-2020 Esri
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

CPPPATH = $$PWD/toolkitcpp/cpp/Esri/ArcGISRuntime/Toolkit

INCLUDEPATH += $$PWD/toolkitcpp/cpp $$CPPPATH

HEADERS += $$CPPPATH/ArcGISAuthenticationController.h \
           $$CPPPATH/AuthenticationController.h \
           $$CPPPATH/BasemapGalleryController.h \
           $$CPPPATH/BasemapGalleryItem.h \
           $$CPPPATH/BookmarksViewController.h \
           $$CPPPATH/BookmarkListItem.h \
           $$CPPPATH/CoordinateConversionConstants.h \
           $$CPPPATH/CoordinateConversionController.h \
           $$CPPPATH/CoordinateConversionOption.h \
           $$CPPPATH/CoordinateConversionResult.h \
           $$CPPPATH/CoordinateOptionDefaults.h \
           $$CPPPATH/FloorFilterController.h \
           $$CPPPATH/FloorFilterFacilityItem.h \
           $$CPPPATH/FloorFilterLevelItem.h \
           $$CPPPATH/FloorFilterSiteItem.h \
           $$CPPPATH/LocatorSearchSource.h \
           $$CPPPATH/Internal/BasemapGalleryImageProvider.h \
           $$CPPPATH/Internal/DisconnectOnSignal.h \
           $$CPPPATH/Internal/DoOnLoad.h \
           $$CPPPATH/Internal/GenericListModel.h \
           $$CPPPATH/Internal/GenericTableProxyModel.h \
           $$CPPPATH/Internal/GeoViews.h \
           $$CPPPATH/Internal/MetaElement.h \
           $$CPPPATH/Internal/SingleShotConnection.h \
           $$CPPPATH/Internal//QmlEnums.h \
           $$CPPPATH/NorthArrowController.h \
           $$CPPPATH/OverviewMapController.h \
           $$CPPPATH/PopupElementViewItem.h \
           $$CPPPATH/PopupViewController.h \
           $$CPPPATH/ScalebarController.h \
           $$CPPPATH/SearchResult.h \
           $$CPPPATH/SearchSourceInterface.h \
           $$CPPPATH/SearchSuggestion.h \
           $$CPPPATH/SearchViewController.h \
           $$CPPPATH/SmartLocatorSearchSource.h \
           $$CPPPATH/TextPopupElementViewController.h \
           $$CPPPATH/TimeSliderController.h \
           $$CPPPATH/UtilityNetworkFunctionTraceResult.h \
           $$CPPPATH/UtilityNetworkFunctionTraceResultsModel.h \
           $$CPPPATH/UtilityNetworkListItem.h \
           $$CPPPATH/UtilityNetworkTraceController.h \
           $$CPPPATH/UtilityNetworkTraceStartingPoint.h \
           $$CPPPATH/UtilityNetworkTraceStartingPointsModel.h

SOURCES += $$CPPPATH/ArcGISAuthenticationController.cpp \
           $$CPPPATH/AuthenticationController.cpp \
           $$CPPPATH/BasemapGalleryController.cpp \
           $$CPPPATH/BasemapGalleryItem.cpp \
           $$CPPPATH/BookmarksViewController.cpp \
           $$CPPPATH/BookmarkListItem.cpp \
           $$CPPPATH/CoordinateConversionConstants.cpp \
           $$CPPPATH/CoordinateConversionController.cpp \
           $$CPPPATH/CoordinateConversionOption.cpp \
           $$CPPPATH/CoordinateConversionResult.cpp \
           $$CPPPATH/CoordinateOptionDefaults.cpp \
           $$CPPPATH/FloorFilterController.cpp \
           $$CPPPATH/FloorFilterFacilityItem.cpp \
           $$CPPPATH/FloorFilterLevelItem.cpp \
           $$CPPPATH/FloorFilterSiteItem.cpp \
           $$CPPPATH/LocatorSearchSource.cpp \
           $$CPPPATH/Internal/BasemapGalleryImageProvider.cpp \
           $$CPPPATH/Internal/GenericListModel.cpp \
           $$CPPPATH/Internal/GenericTableProxyModel.cpp \
           $$CPPPATH/Internal/MetaElement.cpp \
           $$CPPPATH/Internal/QmlEnums.cpp \
           $$CPPPATH/NorthArrowController.cpp \
           $$CPPPATH/OverviewMapController.cpp \
           $$CPPPATH/PopupElementViewItem.cpp \
           $$CPPPATH/PopupViewController.cpp \
           $$CPPPATH/ScalebarController.cpp \
           $$CPPPATH/SearchResult.cpp \
           $$CPPPATH/SearchSourceInterface.cpp \
           $$CPPPATH/SearchSuggestion.cpp \
           $$CPPPATH/SearchViewController.cpp \
           $$CPPPATH/SmartLocatorSearchSource.cpp \
           $$CPPPATH/TextPopupElementViewController.cpp \
           $$CPPPATH/TimeSliderController.cpp \
           $$CPPPATH/UtilityNetworkFunctionTraceResult.cpp \
           $$CPPPATH/UtilityNetworkFunctionTraceResultsModel.cpp \
           $$CPPPATH/UtilityNetworkListItem.cpp \
           $$CPPPATH/UtilityNetworkTraceController.cpp \
           $$CPPPATH/UtilityNetworkTraceStartingPoint.cpp \
           $$CPPPATH/UtilityNetworkTraceStartingPointsModel.cpp
