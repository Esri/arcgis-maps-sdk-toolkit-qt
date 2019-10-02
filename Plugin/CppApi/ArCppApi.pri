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

# This file is a part of the AR features provides in the toolkit.
# This file must be imported in the project to use the C++ API of the
# ArcGIS Runtime SDK for Qt. See AR.md for details.

include($$AR_TOOLKIT_SOURCE_PATH/Common/AR/AR.pri)

HEADERS += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/include/ArcGISArView.h

SOURCES += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/source/ArcGISArView.cpp

INCLUDEPATH += $$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/include
DEPENDPATH += $$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/include
