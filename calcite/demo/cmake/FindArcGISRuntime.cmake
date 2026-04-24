#-------------------------------------------------
#  Copyright 2026 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------
#[================================================[.rst

Find the native ArcGISRuntime includes and libraries.

Hints
^^^^^

A user may set ``ArcGISRuntime_ROOT`` to override the default search path.
See "ArcGISRuntime.cmake" for additional information.

#]================================================]

function(GET_INSTALL_DIR OUT_VAR DESC_VAR)
  # Discover the INI file with an install location (if it exists).
  set(VER_MAJ_MIN ${ArcGISRuntime_FIND_VERSION_MAJOR}.${ArcGISRuntime_FIND_VERSION_MINOR}.${ArcGISRuntime_FIND_VERSION_PATCH} CACHE STRING "Major Minor version ArcGISRuntime" FORCE)
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(INI_PATH $ENV{ALLUSERSPROFILE}/EsriRuntimeQt/ArcGIS\ Runtime\ SDK\ for\ Qt\ ${VER_MAJ_MIN}.ini)
  else()
    set(INI_PATH $ENV{HOME}/.config/EsriRuntimeQt/ArcGIS\ Runtime\ SDK\ for\ Qt\ ${VER_MAJ_MIN}.ini)
  endif()

  # If there is an INI path we attempt to extract the install location from it.
  if(EXISTS ${INI_PATH})
    message("Found configuration file at " ${INI_PATH})
    # Finds the `InstallDir="/path/to/arcgis"` string and extracts the path.
    file(STRINGS ${INI_PATH} INSTALL_INI REGEX InstallDir=)
    string(REGEX REPLACE "^InstallDir=\"(.*)\"$" "\\1" INSTALL_DIR ${INSTALL_INI}) # "
    if(INSTALL_DIR)
      message("Found ArcGISRuntime installation at ${INSTALL_DIR}")
      set(${OUT_VAR} "${INSTALL_DIR}" CACHE STRING ${DESC_VAR})
    else()
      message(FATAL_ERROR "Missing ArcGIS Runtime install location in configuration file. Run the configure script and clear the CMake cache.")
    endif()
  endif()
endfunction()

message("<FindArcGISRuntime.cmake>")

get_install_dir(ArcGISRuntime_INSTALL_DIR "Install directory of ArcGISRuntime")

set(RUNTIME_CMAKE_PATH
  "${ArcGISRuntime_INSTALL_DIR}/sdk/ideintegration/ArcGISRuntime.cmake")

if(EXISTS "${RUNTIME_CMAKE_PATH}")
  include("${RUNTIME_CMAKE_PATH}")
else()
  message(FATAL_ERROR "module ${RUNTIME_CMAKE_PATH} does not exist. Run the configure script and clear the CMake cache.")
endif()

message("</FindArcGISRuntime.cmake>")
