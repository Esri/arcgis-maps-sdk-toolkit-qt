INCLUDEPATH += $$PWD/cpp

CPPPATH = $$PWD/cpp/Esri/ArcGISRuntime/Toolkit

HEADERS += $$CPPPATH/GenericListModel.h \
           $$CPPPATH/NorthArrowController.h \
           $$CPPPATH/register.h

SOURCES += $$CPPPATH/GenericListModel.cpp \
           $$CPPPATH/NorthArrowController.cpp \
           $$CPPPATH/register.cpp

# For Qt Creator only
QML_IMPORT_PATH += $$PWD/qml

RESOURCES += $$PWD/images/toolkit_images.qrc
