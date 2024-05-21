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
# set(TOOLKITUI_PATH ${CMAKE_CURRENT_LIST_DIR})
function(setup_toolkit TARGET_NAME TOOLKIT_DIR)
    set(TOOLKITUI_PATH ${TOOLKIT_DIR}/uitools)

    set(CPP_PATH ${TOOLKITUI_PATH}/cpp/Esri/ArcGISRuntime/Toolkit)

    set(QML_PATH ${TOOLKITUI_PATH}/import/Esri/ArcGISRuntime/Toolkit)

    set(IMG_PATH ${TOOLKITUI_PATH}/images)

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

    set(TOOLKITCOMMON_QML
        ${QML_PATH}/AuthenticationView.qml
        ${QML_PATH}/BasemapGallery.qml
        ${QML_PATH}/BookmarksView.qml
        ${QML_PATH}/Callout.qml
        ${QML_PATH}/ClientCertificateView.qml
        ${QML_PATH}/CoordinateConversion.qml
        # ${QML_PATH}/CurrentVersion.qml
        ${QML_PATH}/FlashImage.qml
        ${QML_PATH}/FloorFilter.qml
        ${QML_PATH}/NorthArrow.qml
        ${QML_PATH}/OAuth2View.qml
        ${QML_PATH}/OverviewMap.qml
        ${QML_PATH}/PopupStackView.qml
        ${QML_PATH}/PopupView.qml
        ${QML_PATH}/Scalebar.qml
        ${QML_PATH}/ScalebarImpl.qml
        ${QML_PATH}/SearchView.qml
        ${QML_PATH}/SslHandshakeView.qml
        ${QML_PATH}/TimeSlider.qml
        ${QML_PATH}/UserCredentialsView.qml
        ${QML_PATH}/UtilityNetworkTrace.qml
        ${QML_PATH}/controller/AuthenticationController.qml
        ${QML_PATH}/controller/BasemapGalleryController.qml
        ${QML_PATH}/controller/BasemapGalleryItem.qml
        ${QML_PATH}/controller/BookmarksViewController.qml
        ${QML_PATH}/controller/CoordinateConversionController.qml
        ${QML_PATH}/controller/CoordinateConversionOption.qml
        ${QML_PATH}/controller/CoordinateConversionResult.qml
        ${QML_PATH}/controller/CurrentVersion.qml
        ${QML_PATH}/controller/FloorFilterController.qml
        ${QML_PATH}/controller/LocatorSearchSource.qml
        ${QML_PATH}/controller/NorthArrowController.qml
        ${QML_PATH}/controller/OverviewMapController.qml
        ${QML_PATH}/controller/PopupViewController.qml
        ${QML_PATH}/controller/ScalebarController.qml
        ${QML_PATH}/controller/SearchViewController.qml
        ${QML_PATH}/controller/SmartLocatorSearchSource.qml
        ${QML_PATH}/controller/TimeSliderController.qml)

    set(TOOLKITCOMMON_IMAGES
        ${IMG_PATH}/basemap.svg
        ${IMG_PATH}/chevron-down.svg
        ${IMG_PATH}/chevron-left.svg
        ${IMG_PATH}/chevron-up.svg
        ${IMG_PATH}/chevrons-left.svg
        ${IMG_PATH}/chevrons-right.svg
        ${IMG_PATH}/compass.svg
        ${IMG_PATH}/cursor-click.svg
        ${IMG_PATH}/edit-attributes.svg
        ${IMG_PATH}/ellipsis.svg
        ${IMG_PATH}/flash.svg
        ${IMG_PATH}/forward.svg
        ${IMG_PATH}/information.svg
        ${IMG_PATH}/layer-zoom-to.svg
        ${IMG_PATH}/organization.svg
        ${IMG_PATH}/pause.svg
        ${IMG_PATH}/pin-tear.svg
        ${IMG_PATH}/pin.svg
        ${IMG_PATH}/play.svg
        ${IMG_PATH}/plus-circle.svg
        ${IMG_PATH}/reverse.svg
        ${IMG_PATH}/search.svg
        ${IMG_PATH}/trash.svg
        ${IMG_PATH}/x.svg
        ${IMG_PATH}/zoom-to-object.svg)

    set(TOOLKITREGISTER_HEADERS
        ${REGISTER_PATH}/register.h
        ${REGISTER_PATH}/internal/register_cpp.h)

    set(TOOLKITREGISTER_SOURCES
        ${REGISTER_PATH}/register.cpp
        ${REGISTER_PATH}/internal/register_cpp.cpp)

    foreach(QML_FILE ${TOOLKITCOMMON_QML})
        get_filename_component(QML_FILE_NAME ${QML_FILE} NAME)
        set_source_files_properties(
            ${QML_FILE}
            PROPERTIES QT_RESOURCE_ALIAS ${QML_FILE_NAME})
    endforeach()

    foreach(IMG_FILE ${TOOLKITCOMMON_IMAGES})
        get_filename_component(IMG_FILE_NAME ${IMG_FILE} NAME)
        set_source_files_properties(
            ${IMG_FILE}
            PROPERTIES QT_RESOURCE_ALIAS images/${IMG_FILE_NAME})
    endforeach()

    qt_add_qml_module(toolkitlib
        URI Esri.ArcGISRuntime.Toolkit
        RESOURCE_PREFIX /esri.com/imports
        RESOURCES ${TOOLKITCOMMON_IMAGES}
        QML_FILES ${TOOLKITCOMMON_QML}
        SOURCES ${TOOLKITCOMMON_HEADERS} ${TOOLKITCOMMON_SOURCES} ${TOOLKITREGISTER_HEADERS} ${TOOLKITREGISTER_SOURCES})

    target_compile_definitions(toolkitlib PRIVATE CPP_ARCGISRUNTIME_TOOLKIT)

    find_package(Qt6 COMPONENTS REQUIRED QuickControls2 WebView Svg)

    target_link_libraries(toolkitlib PRIVATE
        Qt6::QuickControls2
        Qt6::WebView
        Qt6::Svg
        ArcGISRuntime::Cpp)

    target_link_libraries(${TARGET_NAME} PRIVATE toolkitlib)

    target_include_directories(toolkitlib PUBLIC
        ${TOOLKITUI_PATH}/cpp
        ${TOOLKITUI_PATH}/register
        ${CPP_PATH}
        ${REGISTER_PATH})

    target_include_directories(${TARGET_NAME} PRIVATE toolkitlib)

endfunction()


