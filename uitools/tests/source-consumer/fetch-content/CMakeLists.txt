cmake_minimum_required(VERSION 3.16)

project(ArcGISRuntimeToolkitFetchContentConsumer LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../examples/cmake")
find_package(Qt6 6.8.2 REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg WebView Graphs NetworkAuth)
find_package(ArcGISRuntime 300.2.0 REQUIRED COMPONENTS Cpp)

include(FetchContent)
set(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK ON)
FetchContent_Declare(
    arcgis_runtime_toolkit
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../.."
    SOURCE_SUBDIR uitools
)
FetchContent_MakeAvailable(arcgis_runtime_toolkit)

qt_add_executable(ArcGISRuntimeToolkitFetchContentConsumer
    main.cpp
)

target_link_libraries(ArcGISRuntimeToolkitFetchContentConsumer PRIVATE
    Qt6::Gui
    Qt6::Qml
    ArcGISRuntimeToolkit::Quick
)
