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

SOURCES += NorthArrow\NorthArrowController.cpp

HEADERS += NorthArrow\NorthArrowController.h

DISTFILES = qmldir \
    ../shared/qml/NorthArrow.qml \

INCLUDEPATH += NorthArrow

pluginfiles.files += \
    ../shared/qml/NorthArrow.qml

DEFINES += NORTHARROW
