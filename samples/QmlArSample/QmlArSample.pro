#-------------------------------------------------
#  Copyright 2019 ESRI
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

CONFIG += c++11

QT += core gui opengl network positioning sensors qml quick

ARCGIS_RUNTIME_VERSION = 100.6
include($$PWD/arcgisruntime.pri)

TEMPLATE = app
TARGET = QmlArSample

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) { 
        error("$$TARGET requires Qt 5.12.0")
    }
	equals(QT_MINOR_VERSION, 12) : lessThan(QT_PATCH_VERSION, 0) {
		error("$$TARGET requires Qt 5.12.0")
	}
}

#-------------------------------------------------------------------------------

HEADERS += \
    AppInfo.h

SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

OTHER_FILES += \
    wizard.xml \
    wizard.png \
    qml/*.qml \
    qml/*.js

#-------------------------------------------------------------------------------

# AR configuration
# The path to the toolkit source correspond to the files downloaded from the GitHub.

AR_TOOLKIT_SOURCE_PATH = /Users/guil8553/applications/qt/sdk/toolkit/Plugin/
include($$AR_TOOLKIT_SOURCE_PATH/ArQmlApi.pri)

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

