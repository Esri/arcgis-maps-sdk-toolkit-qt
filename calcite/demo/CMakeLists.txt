#-------------------------------------------------
#  Copyright 2012-2026 ESRI
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#-------------------------------------------------
cmake_minimum_required(VERSION 3.16)

project(calcite_test LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(IOS)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "16.0" CACHE STRING "Minimum iOS deployment version" FORCE)
elseif(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum macOS deployment version" FORCE)
endif()

find_package(Qt6 COMPONENTS REQUIRED Core Quick Qml OpenGL QuickControls2)
if(Qt6Core_VERSION VERSION_LESS 6.8.2)
  message(FATAL_ERROR "This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.8.2")
endif()
find_package(ArcGISRuntime 300.1.0 COMPONENTS REQUIRED Cpp)

set(HEADERS
  CalciteTest.h)

set(SOURCES
  main.cpp
  CalciteTest.cpp)

set(RESOURCES
  qml/qml.qrc
  Resources/Resources.qrc
  "${CMAKE_CURRENT_SOURCE_DIR}/../Calcite/calcite.qrc")

# Optional: Calcite Icons from submodule (only if submodule is initialized)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../calcite-design-system/packages/ui-icons/icons")
  list(APPEND RESOURCES Resources/calcite-icons.qrc)
  message(STATUS "Calcite icons submodule detected - including icon resources")
else()
  message(STATUS "Calcite icons submodule not initialized. Run: git submodule update --init --recursive")
endif()

set(SOURCE_FILES
  ${HEADERS}
  ${SOURCES}
  ${RESOURCES}
  $<$<BOOL:${WIN32}>:Win/Resources.rc>
  $<$<BOOL:${APPLE}>:Mac/AppIcon.icns>)

if(ANDROID)
  qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
else()
  qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES})
  # IOS uses static Runtimecore Framework
  if(IOS)
    # Workaround for the CMake version of https://bugreports.qt.io/browse/QTBUG-129651
    qt_add_ios_ffmpeg_libraries(${PROJECT_NAME})
    get_target_property(Qt6Core_LOCATION Qt6::Core LOCATION)
    get_filename_component(Qt6_DIR ${Qt6Core_LOCATION} DIRECTORY)
    set(FFMPEG_LIB_DIR "${Qt6_DIR}/../ffmpeg")
    if(Qt6Core_VERSION VERSION_LESS "6.8.3")
      set(FRAMEWORK "framework")
    else()
      set(FRAMEWORK "xcframework")
    endif()

    set(FFMPEG_FRAMEWORKS
      "${FFMPEG_LIB_DIR}/libavcodec.${FRAMEWORK}"
      "${FFMPEG_LIB_DIR}/libavformat.${FRAMEWORK}"
      "${FFMPEG_LIB_DIR}/libavutil.${FRAMEWORK}"
      "${FFMPEG_LIB_DIR}/libswresample.${FRAMEWORK}"
      "${FFMPEG_LIB_DIR}/libswscale.${FRAMEWORK}")
    set_target_properties(${PROJECT_NAME} PROPERTIES
      INSTALL_RPATH "@executable_path/Frameworks;@loader_path/Frameworks"
      XCODE_EMBED_FRAMEWORKS "${ArcGISRuntime_runtimecore_LIB};${FFMPEG_FRAMEWORKS}"
      XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
      LINK_FLAGS "-F${FFMPEG_LIB_DIR} -F${ArcGISRuntime_runtimecore_LIB_DIR} -Wl,-rpath,@executable_path -Wl,-rpath,@loader_path/Frameworks")
  endif()
  # On MacOSX add icon to app bundle.
  set(MACOSX_BUNDLE_ICON_FILE AppIcon.icns)
  set_source_files_properties(Mac/AppIcon.icns
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

  # Copy required dynamic libraries to the build folder as a post-build step.
  if(DEFINED ArcGISRuntime_LIBRARIES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${ArcGISRuntime_LIBRARIES}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  endif()
endif()

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Qml
  Qt6::OpenGL
  Qt6::QuickControls2
  ArcGISRuntime::Cpp)

if(ANDROID)
  set(PROJECT_DEPLOYABLE_LIBS
    ${ArcGISRuntime_LIBRARIES})

  list(JOIN PROJECT_DEPLOYABLE_LIBS , PROJECT_DEPLOYABLE_LIBS_STRING)

  add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${ArcGISRuntime_MapsSDKNative_JAR}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/android-build-${PROJECT_NAME}/libs/MapsSDKNative.jar")

  # Setup openssl by cloning the repo https://github.com/KDAB/android_openssl
  # Uncomment line below and point to your openssl path.
  #include(<your_path_to_android_openssl>/CMakeLists.txt)

  set_property(TARGET ${PROJECT_NAME} PROPERTY QT_ANDROID_EXTRA_LIBS ${PROJECT_DEPLOYABLE_LIBS_STRING} ${ANDROID_EXTRA_LIBS})
  get_property(EXTRA_LIBS TARGET ${PROJECT_NAME} PROPERTY QT_ANDROID_EXTRA_LIBS)
  if(NOT EXTRA_LIBS MATCHES "libssl" OR NOT EXTRA_LIBS MATCHES "libcrypto")
      message(WARNING "openssl libraries are missing, check the project CMakeLists.txt and set up openssl environment")
  endif()
endif()