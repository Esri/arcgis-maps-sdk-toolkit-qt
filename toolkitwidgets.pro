INCLUDEPATH += $$PWD/widgets $$PWD/common

WIDGETPATH = $$PWD/widgets/Esri/ArcGISRuntime/Toolkit

HEADERS += $$WIDGETPATH/NorthArrow.h \
           $$WIDGETPATH/register.h

SOURCES += $$WIDGETPATH/NorthArrow.cpp

RESOURCES += $$PWD/images/toolkit_images.qrc
