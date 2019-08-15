#-------------------------------------------------
#  Copyright 2019 ESRI
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

# Defines the version of the ArcGIS Runtime SDK which the toolkit is built for:
ARCGIS_RUNTIME_VERSION = 100.6

# Set this to your desired output location for toolkit builds:
TOOLKIT_INSTALL = $$(PWD)../output

# Shared image resources
RESOURCES += \
    ../shared/images/images.qrc
