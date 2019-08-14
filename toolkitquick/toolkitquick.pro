TEMPLATE = lib
TARGET = toolkitquick
QT += qml quick
CONFIG += plugin c++11 staticlib

#TARGET = $$qtLibraryTarget($$TARGET)
uri = esri.arcgisruntime.toolkitquick

# Input
SOURCES += \
        toolkitquick_plugin.cpp \
        NorthArrowController.cpp

HEADERS += \
        toolkitquick_plugin.h \
        NorthArrowController.h

DISTFILES = qmldir \
    qml/NorthArrow.qml

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}


pluginfiles.files += \
    qmldir \
    qml/NorthArrow.qml


qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir pluginfiles
}

INSTALLS += target pluginfiles

QMAKE_MOC_OPTIONS += -Muri=esri.arcgisruntime.toolkitquick

RESOURCES += \
    ../images/images.qrc
