include($$PWD/common.pri)

INCLUDEPATH += $$PWD/quick
HEADERS += $$PWD/quick/Esri/ArcGISRuntime/Toolkit/register.h
SOURCES += $$PWD/quick/Esri/ArcGISRuntime/Toolkit/register.cpp

# For Qt Creator only
QML_IMPORT_PATH += $$PWD/import