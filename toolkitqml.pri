
DEFINES += QML_TOOLKIT

TOOLKIT_MEMBERS = $$files($$PWD/*)
for (component, TOOLKIT_MEMBERS) {
  message("Processing component: " $$component)

  path = $$absolute_path($$component)

  exists($$path/images/images.qrc) {
    RESOURCES += $$path/images/images.qrc
  }

  exists($$path/qml_controllers/qml_controllers.qrc) {
    RESOURCES += $$path/qml_controllers/qml_controllers.qrc
  }

  exists($$path/qml_views/qml_views.qrc) {
    RESOURCES += $$path/qml_views/qml_views.qrc
  }
}
