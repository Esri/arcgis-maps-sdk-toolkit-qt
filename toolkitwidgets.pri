
DEFINES += WIDGETS_TOOLKIT

TOOLKIT_MEMBERS = $$files($$PWD/*)
for (component, TOOLKIT_MEMBERS) {
  message("Processing component: " $$component)

  path = $$absolute_path($$component)

  exists($$path/cpp_views/cpp_views.pri) {
    include($$path/cpp_views/cpp_views.pri)
  }

  exists($$path/cpp_controllers/cpp_controllers.pri) {
    include($$path/cpp_controllers/cpp_controllers.pri)
  }

  exists($$path/images/images.qrc) {
    RESOURCES += $$path/images/images.qrc
  }
}

