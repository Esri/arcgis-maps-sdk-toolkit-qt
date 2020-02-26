include($$PWD/common.pri)

INCLUDEPATH += $$PWD/widgets

WIDGETPATH = $$PWD/widgets/Esri/ArcGISRuntime/Toolkit

HEADERS += $$WIDGETPATH/CoordinateConversion.h \
           $$WIDGETPATH/NorthArrow.h

SOURCES += $$WIDGETPATH/CoordinateConversion.cpp \
           $$WIDGETPATH/NorthArrow.cpp

FORMS += $$WIDGETPATH/CoordinateConversion.ui

RESOURCES += $$PWD/import/Esri/ArcGISRuntime/Toolkit/images/toolkit_images.qrc