TEMPLATE = lib
TARGET = toolkit
QT += qml quick
CONFIG += plugin c++11 staticlib

# Toolkit configuration settings
include($$PWD/../toolkit_config.pri)

# Esri ArcGIS Runtime SDK
include($$PWD/arcgisruntime.pri)

uri = esri.arcgisruntime.toolkit

# Input
SOURCES += \
        toolkitqml_plugin.cpp

HEADERS += \
        toolkitqml_plugin.h

DISTFILES = qmldir

# TOOLS
include($${PWD}/NorthArrow/NorthArrow.pri)

pluginfiles.files += \
    qmldir

qmldir.files = qmldir

TOOLKITQML_INSTALL = $${TOOLKIT_INSTALL}/toolkitqml

target.path += $${TOOLKITQML_INSTALL}/imports/esri/arcgisruntime/toolkit
pluginfiles.path += $${TOOLKITQML_INSTALL}/imports/esri/arcgisruntime/toolkit

INSTALLS += target pluginfiles

QMAKE_MOC_OPTIONS += -Muri=$${uri}


