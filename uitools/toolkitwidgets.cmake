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
    set(TOOLKITUI_PATH ${TOOLKIT_DIR}/uitools)
    set(CPP_PATH ${TOOLKITUI_PATH}/cpp/Esri/ArcGISRuntime/Toolkit)
    set(WIDGET_PATH ${TOOLKITUI_PATH}/widgets/Esri/ArcGISRuntime/Toolkit)

    setup_common_vars(TOOLKITCOMMON_HEADERS TOOLKITCOMMON_SOURCES)
    setup_widget_vars(TOOLKITWIDGET_HEADERS TOOLKITWIDGET_SOURCES)
    setup_forms_vars(TOOLKITWIDGET_FORMS)

    target_include_directories(${TARGET_NAME} PRIVATE
        ${TOOLKITUI_PATH}/cpp
        ${TOOLKITUI_PATH}/widgets
        ${CPP_PATH}
        ${WIDGET_PATH})

    target_sources(${TARGET_NAME} PRIVATE
        ${TOOLKITCOMMON_HEADERS}
        ${TOOLKITCOMMON_SOURCES}
        ${TOOLKITWIDGET_HEADERS}
        ${TOOLKITWIDGET_SOURCES}
        ${TOOLKITWIDGET_FORMS}
        ${TOOLKITUI_PATH}/images/esri_arcgisruntime_toolkit_images.qrc)

    find_package(Qt6 COMPONENTS REQUIRED Widgets WebEngineWidgets Svg)

    target_link_libraries(${TARGET_NAME} PRIVATE
        Qt::Widgets
        Qt::WebEngineWidgets
        Qt::Svg)

    target_compile_definitions(${TARGET_NAME} PRIVATE WIDGETS_ARCGISRUNTIME_TOOLKIT)
endfunction()

function(setup_common_vars HEADERS SOURCES)
    set(${HEADERS}
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
        ${CPP_PATH}/UtilityNetworkTraceStartingPointsModel.h
        PARENT_SCOPE)

    set(${SOURCES}
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
        ${CPP_PATH}/UtilityNetworkTraceStartingPointsModel.cpp
        PARENT_SCOPE)
endfunction()

function(setup_widget_vars HEADERS SOURCES)
    set(${HEADERS}
        ${WIDGET_PATH}/AuthenticationView.h
        ${WIDGET_PATH}/BasemapGallery.h
        ${WIDGET_PATH}/BookmarksView.h
        ${WIDGET_PATH}/CoordinateConversion.h
        ${WIDGET_PATH}/FloorFilter.h
        ${WIDGET_PATH}/NorthArrow.h
        ${WIDGET_PATH}/OverviewMap.h
        ${WIDGET_PATH}/Internal/ClientCertificatePasswordDialog.h
        ${WIDGET_PATH}/Internal/ClientCertificateView.h
        ${WIDGET_PATH}/Internal/CoordinateEditDelegate.h
        ${WIDGET_PATH}/Internal/Flash.h
        ${WIDGET_PATH}/Internal/OAuth2View.h
        ${WIDGET_PATH}/Internal/SslHandshakeView.h
        ${WIDGET_PATH}/Internal/UserCredentialView.h
        PARENT_SCOPE)

    set(${SOURCES}
        ${WIDGET_PATH}/AuthenticationView.cpp
        ${WIDGET_PATH}/BasemapGallery.cpp
        ${WIDGET_PATH}/BookmarksView.cpp
        ${WIDGET_PATH}/CoordinateConversion.cpp
        ${WIDGET_PATH}/FloorFilter.cpp
        ${WIDGET_PATH}/NorthArrow.cpp
        ${WIDGET_PATH}/OverviewMap.cpp
        ${WIDGET_PATH}/Internal/ClientCertificatePasswordDialog.cpp
        ${WIDGET_PATH}/Internal/ClientCertificateView.cpp
        ${WIDGET_PATH}/Internal/CoordinateEditDelegate.cpp
        ${WIDGET_PATH}/Internal/Flash.cpp
        ${WIDGET_PATH}/Internal/OAuth2View.cpp
        ${WIDGET_PATH}/Internal/SslHandshakeView.cpp
        ${WIDGET_PATH}/Internal/UserCredentialView.cpp
        PARENT_SCOPE)
endfunction()

function(setup_forms_vars FORMS)
    set(${FORMS}
        ${WIDGET_PATH}/AuthenticationView.ui
        ${WIDGET_PATH}/BasemapGallery.ui
        ${WIDGET_PATH}/BookmarksView.ui
        ${WIDGET_PATH}/CoordinateConversion.ui
        ${WIDGET_PATH}/FloorFilter.ui
        ${WIDGET_PATH}/OverviewMap.ui
        ${WIDGET_PATH}/Internal/ClientCertificatePasswordDialog.ui
        ${WIDGET_PATH}/Internal/ClientCertificateView.ui
        ${WIDGET_PATH}/Internal/OAuth2View.ui
        ${WIDGET_PATH}/Internal/SslHandshakeView.ui
        ${WIDGET_PATH}/Internal/UserCredentialView.ui
        PARENT_SCOPE)
endfunction()
