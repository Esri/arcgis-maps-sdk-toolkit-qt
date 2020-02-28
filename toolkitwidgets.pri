include($$PWD/common.pri)

INCLUDEPATH += $$PWD/widgets

WIDGETPATH = $$PWD/widgets/Esri/ArcGISRuntime/Toolkit

HEADERS += $$WIDGETPATH/CoordinateEditDelegate.h \
           $$WIDGETPATH/CoordinateConversion.h \
           $$WIDGETPATH/Flash.h \
           $$WIDGETPATH/NorthArrow.h

SOURCES += $$WIDGETPATH/CoordinateEditDelegate.cpp \ 
           $$WIDGETPATH/CoordinateConversion.cpp \
           $$WIDGETPATH/Flash.cpp \
           $$WIDGETPATH/NorthArrow.cpp

FORMS += $$WIDGETPATH/CoordinateConversion.ui

RESOURCES += $$PWD/import/Esri/ArcGISRuntime/Toolkit/images/toolkit_images.qrc

DEFINES += WIDGETS_ARCGISRUNTIME_TOOLKIT