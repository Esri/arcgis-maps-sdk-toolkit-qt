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
include($$PWD/NorthArrow/NorthArrow.pri)

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

pluginfiles.files += \
    qmldir

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir pluginfiles
}

TOOLKITQUICK_INSTALL = $${TOOLKIT_INSTALL}/toolkitquick

target.path += $${TOOLKITQUICK_INSTALL}/imports/esri/arcgisruntime/toolkit
pluginfiles.path += $${TOOLKITQUICK_INSTALL}/imports/esri/arcgisruntime/toolkit

INSTALLS += target pluginfiles

QMAKE_MOC_OPTIONS += -Muri=$${uri}

RESOURCES += \
    ../images/images.qrc
