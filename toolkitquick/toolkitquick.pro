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
        toolkitquick_plugin.cpp

HEADERS += \
        toolkitquick_plugin.h

DISTFILES = qmldir

### TOOLS
include($${PWD}/NorthArrow/NorthArrow.pri)

INCLUDEPATH += \
            ../shared/cpp\

pluginfiles.files += \
    qmldir

qmldir.files = qmldir

TOOLKITQUICK_INSTALL = $${TOOLKIT_INSTALL}/toolkitquick

target.path += $${TOOLKITQUICK_INSTALL}/imports/esri/arcgisruntime/toolkit
pluginfiles.path += $${TOOLKITQUICK_INSTALL}/imports/esri/arcgisruntime/toolkit

INSTALLS += target pluginfiles

QMAKE_MOC_OPTIONS += -Muri=$${uri}
