cmake_minimum_required(VERSION 3.16)

project(ArcGISRuntimeToolkitWidgetsConsumer LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../examples/cmake")
find_package(Qt6 6.8.2 REQUIRED COMPONENTS Core Gui OpenGLWidgets Svg WebEngineWidgets Widgets Graphs NetworkAuth)
find_package(ArcGISRuntime 300.2.0 REQUIRED COMPONENTS Cpp)

set(ARCGIS_RUNTIME_TOOLKIT_BUILD_WIDGETS ON)
add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/../../../"
    "${CMAKE_CURRENT_BINARY_DIR}/arcgis-runtime-toolkit"
)

qt_add_executable(ArcGISRuntimeToolkitWidgetsConsumer
    main.cpp
)

target_link_libraries(ArcGISRuntimeToolkitWidgetsConsumer PRIVATE
    Qt6::Widgets
    ArcGISRuntimeToolkit::Widgets
)
