import QtQuick 2.12

Rectangle {
    id: handle

    property var palette
    property bool pressed
    property bool hovered
    property bool vertical
    property bool visualFocus
    property bool lingeringFocus

    implicitWidth: 18
    implicitHeight: 18
    radius: 50
    rotation: handle.vertical ? -90 : 0
    color: handle.pressed ? Calcite.brandHover : handle.lingeringFocus || mouseArea.containsMouse ? Calcite.brand : Calcite.foreground1
    border {
        width: 2
        color: handle.lingeringFocus || handle.hovered ? Calcite.brand : Calcite.borderInput
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onPositionChanged: mouse.accepted = false
    }
}
