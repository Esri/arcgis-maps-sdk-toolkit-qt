cmake_minimum_required(VERSION 3.16)

project(ArcGISRuntimeToolkitAddSubdirectoryConsumer LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../examples/cmake")
find_package(Qt6 6.8.2 REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg WebView Graphs NetworkAuth)
find_package(ArcGISRuntime 300.2.0 REQUIRED COMPONENTS Cpp)

set(ARCGIS_RUNTIME_TOOLKIT_BUILD_QUICK ON)
add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/../../../"
    "${CMAKE_CURRENT_BINARY_DIR}/arcgis-runtime-toolkit"
)

qt_add_executable(ArcGISRuntimeToolkitAddSubdirectoryConsumer
    main.cpp
)

target_link_libraries(ArcGISRuntimeToolkitAddSubdirectoryConsumer PRIVATE
    Qt6::Gui
    Qt6::Qml
    ArcGISRuntimeToolkit::Quick
)
