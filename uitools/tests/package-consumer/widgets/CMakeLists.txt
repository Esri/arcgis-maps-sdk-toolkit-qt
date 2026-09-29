cmake_minimum_required(VERSION 3.16)

project(ArcGISRuntimeToolkitWidgetsConsumer LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../examples/cmake")

find_package(Qt6 6.8.2 REQUIRED COMPONENTS Core Gui Widgets)
find_package(ArcGISRuntime 300.2.0 REQUIRED COMPONENTS Cpp)
find_package(ArcGISRuntimeToolkit 300.2 CONFIG REQUIRED COMPONENTS Widgets)

qt_add_executable(ArcGISRuntimeToolkitWidgetsConsumer
    main.cpp
)

target_link_libraries(ArcGISRuntimeToolkitWidgetsConsumer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ArcGISRuntimeToolkit::Widgets
)
