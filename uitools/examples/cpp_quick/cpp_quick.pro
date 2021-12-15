#-------------------------------------------------
#  Copyright 2021 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TEMPLATE = app

CONFIG += c++14 qmltypes

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick quickcontrols2 webview

QML_IMPORT_NAME = "DemoApp"
QML_IMPORT_MAJOR_VERSION = 1

TARGET = cpp_quick

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.2")
    }
        equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 2) {
                error("$$TARGET requires Qt 5.15.2")
        }
}

equals(QT_MAJOR_VERSION, 6) {
  error("This version of the ArcGIS Runtime SDK for Qt is incompatible with Qt 6")
}

ARCGIS_RUNTIME_VERSION = 100.14
include($$PWD/arcgisruntime.pri)
include($$PWD/../../toolkitcpp.pri)

INCLUDEPATH += $$PWD/src

HEADERS += \
    src/BaseDemo.h

SOURCES += \
    src/main.cpp \
    src/BaseDemo.cpp

RESOURCES += \
    qtquick.qrc \
    $$absolute_path($$PWD/../../../calcite-qml/Calcite/calcite.qrc) \
    $$absolute_path($$PWD/../shared/qml/shared_demo_qml.qrc)


include(src/demos/demos.pri)
include(src/proxies/proxies.pri)

#-------------------------------------------------------------------------------

win32 {
    include (Win/Win.pri)
}

macx {
    include (Mac/Mac.pri)
}

ios {
    include (iOS/iOS.pri)
}

android {
    include (Android/Android.pri)
}
