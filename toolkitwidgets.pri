
include($$PWD/toolkitcpp.pri)

INCLUDEPATH += $$PWD/widgets

WIDGETPATH = $$PWD/widgets/Esri/ArcGISRuntime/Toolkit

HEADERS += $$WIDGETPATH/NorthArrow.h

SOURCES += $$WIDGETPATH/NorthArrow.cpp

RESOURCES += $$PWD/import/Esri/ArcGISRuntime/Toolkit/images/toolkit_images.qrc