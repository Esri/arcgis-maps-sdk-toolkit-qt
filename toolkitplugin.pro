TEMPLATE = lib
CONFIG += qt plugin
QT += qml quick

INCLUDEPATH += plugin common quick/src
DESTDIR = imports/Esri/ArcGISRuntime/Toolkit
TARGET = ArcGISRuntimeToolkitPlugin
RESOURCES += images/images.qrc
ARCGIS_RUNTIME_VERSION = 100.7

include(arcgisruntime.pri)
include(common/common.pri)
include(plugin/plugin.pri)
include(quick/quick.pri)


TOOLKIT_DESTDIR = $$OUT_PWD/$$DESTDIR
export(TOOLKIT_DESTDIR)

# Do a post-build copy step of our required QML libs.
copydata.commands = $(COPY_DIR) $$PWD/quick/qml/* $$TOOLKIT_DESTDIR
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
