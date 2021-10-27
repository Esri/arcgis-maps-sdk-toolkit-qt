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

mac {
    cache()
}

#-------------------------------------------------------------------------------

CONFIG += c++14

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick quickcontrols2

ARCGIS_RUNTIME_VERSION = 100.13
include($$PWD/arcgisruntime.pri)

TEMPLATE = app
TARGET = calite_test

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

#-------------------------------------------------------------------------------

QML_IMPORT_PATH += /Users/neil9879/codebases/calcite-qml

SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc \
    $$absolute_path($$PWD/../Calcite/calcite.qrc)

OTHER_FILES += \
    wizard.xml \
    wizard.png

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