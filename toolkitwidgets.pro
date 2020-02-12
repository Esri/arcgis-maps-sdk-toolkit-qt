
TEMPLATE = lib
CONFIG += qt
QT += widgets

INCLUDEPATH += common widgets
TARGET = ArcGISRuntimeToolkitWidgets
RESOURCES += images/images.qrc
ARCGIS_RUNTIME_VERSION = 100.7

include(arcgisruntime.pri)
include(common/common.pri)
include(widgets/widgets.pri)
