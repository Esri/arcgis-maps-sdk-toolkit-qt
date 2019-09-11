
DEFINES += QML_TOOLKIT

TOOLKIT_MEMBERS = $$files($$PWD/*)
for (component, TOOLKIT_MEMBERS) {
  message("Processing component: " $$component)

  path = $$absolute_path($$component)

  exists($$path/qml_controllers/qml_controllers.pri) {
    include($$path/qml_controllers/qml_controllers.pri)
  }

  exists($$path/qml_views/qml_views.pri) {
    include($$path/qml_views/qml_views.pri)
  }
}

exists(images/images.qrc) {
  RESOURCES += $$PWD/images/images.qrc
}

