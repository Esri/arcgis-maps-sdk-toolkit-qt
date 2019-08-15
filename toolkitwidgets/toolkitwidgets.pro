#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T11:01:40
#
#-------------------------------------------------

QT += opengl widgets

TARGET = toolkit
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Toolkit configuration settings
include($$PWD/../toolkit_config.pri)

# Esri ArcGIS Runtime SDK
include($$PWD/arcgisruntime.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

### TOOLS
include($$PWD/NorthArrow/NorthArrow.pri)

INCLUDEPATH += \
            ../shared/cpp\

TOOLKITWIDGETS_INSTALL = $${TOOLKIT_INSTALL}/toolkitwidgets

target.path += $${TOOLKITWIDGETS_INSTALL}/lib
includes.path += $${TOOLKITWIDGETS_INSTALL}/include

INSTALLS += includes target

DEFINES += WIDGETS_TOOLKIT
