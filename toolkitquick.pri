
DEFINES += QUICK_TOOLKIT

INCLUDEPATH += $$PWD/plugin
HEADERS += $$PWD/plugin/register_plugins.h
SOURCES += $$PWD/plugin/register_plugins.cpp

TOOLKIT_MEMBERS = $$files($$PWD/*)
for (component, TOOLKIT_MEMBERS) {
  message("Processing component: " $$component)

  path = $$absolute_path($$component)

  exists($$path/cpp_controllers/cpp_controllers.pri) {
    include($$path/cpp_controllers/cpp_controllers.pri)
  }

  exists($$path/qml_views/qml_views.pri) {
    include($$path/qml_views/qml_views.pri)
  }
}

exists(images/images.qrc) {
  RESOURCES += $$PWD/images/images.qrc
}

