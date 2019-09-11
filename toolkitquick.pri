
DEFINES += QUICK_TOOLKIT

INCLUDEPATH += $$PWD/plugin
HEADERS += $$PWD/plugin/register_plugins.h
SOURCES += $$PWD/plugin/register_plugins.cpp

include($$PWD/cpp_controllers/cpp_controllers.pri)

RESOURCES += $$PWD/qml_views/qml_views.qrc
RESOURCES += $$PWD/images/images.qrc

