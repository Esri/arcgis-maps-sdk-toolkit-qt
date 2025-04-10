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
project(libtoolkitwidgets VERSION 1.0 LANGUAGES CXX)

include(../common/CMakeLists.txt)

set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Svg WebEngineWidgets Widgets)

qt_standard_project_setup()

qt_add_library(libtoolkitwidgets STATIC
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_HEADERS}
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_SOURCES}

    # public headers
    src/Esri/ArcGISRuntime/Toolkit/AuthenticationView.h
    src/Esri/ArcGISRuntime/Toolkit/BasemapGallery.h
    src/Esri/ArcGISRuntime/Toolkit/BookmarksView.h
    src/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.h
    src/Esri/ArcGISRuntime/Toolkit/FloorFilter.h
    src/Esri/ArcGISRuntime/Toolkit/NorthArrow.h
    src/Esri/ArcGISRuntime/Toolkit/OverviewMap.h

    # public sources
    src/Esri/ArcGISRuntime/Toolkit/AuthenticationView.cpp
    src/Esri/ArcGISRuntime/Toolkit/BasemapGallery.cpp
    src/Esri/ArcGISRuntime/Toolkit/BookmarksView.cpp
    src/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.cpp
    src/Esri/ArcGISRuntime/Toolkit/FloorFilter.cpp
    src/Esri/ArcGISRuntime/Toolkit/NorthArrow.cpp
    src/Esri/ArcGISRuntime/Toolkit/OverviewMap.cpp

    # public UI
    src/Esri/ArcGISRuntime/Toolkit/AuthenticationView.ui
    src/Esri/ArcGISRuntime/Toolkit/BasemapGallery.ui
    src/Esri/ArcGISRuntime/Toolkit/BookmarksView.ui
    src/Esri/ArcGISRuntime/Toolkit/CoordinateConversion.ui
    src/Esri/ArcGISRuntime/Toolkit/FloorFilter.ui
    src/Esri/ArcGISRuntime/Toolkit/OverviewMap.ui

    # internal headers
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/CoordinateEditDelegate.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/Flash.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.h
    src/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.h

    # internal sources
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/CoordinateEditDelegate.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/Flash.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.cpp
    src/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.cpp

    # internal UI
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificatePasswordDialog.ui
    src/Esri/ArcGISRuntime/Toolkit/Internal/ClientCertificateView.ui
    src/Esri/ArcGISRuntime/Toolkit/Internal/OAuth2View.ui
    src/Esri/ArcGISRuntime/Toolkit/Internal/SslHandshakeView.ui
    src/Esri/ArcGISRuntime/Toolkit/Internal/UserCredentialView.ui
)

target_include_directories(libtoolkitwidgets PUBLIC
    ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_INCLUDES}
    src
)

target_compile_definitions(libtoolkitwidgets PRIVATE
    WIDGETS_ARCGISRUNTIME_TOOLKIT
)

target_link_libraries(libtoolkitwidgets PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Svg
    Qt6::WebEngineWidgets
    Qt6::Widgets
    ArcGISRuntime::Cpp
)

qt_add_resources(libtoolkitwidgets "ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES"
    PREFIX
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_PREFIX}
    BASE
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES_BASE}
    FILES
        ${ESRI_ARCGISRUNTIME_TOOLKIT_COMMON_IMAGES}
)
