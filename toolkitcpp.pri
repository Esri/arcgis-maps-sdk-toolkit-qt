INCLUDEPATH += $$PWD/quick $$PWD/common

QUICKPATH = $$PWD/quick/Esri/ArcGISRuntime/Toolkit

HEADERS += $$QUICKPATH/NorthArrowController.h \
           $$QUICKPATH/register.h

SOURCES += $$QUICKPATH/NorthArrowController.cpp \
           $$QUICKPATH/register.cpp

QML_IMPORT_PATH += $$PWD/qml

RESOURCES += $$PWD/images/toolkit_images.qrc
