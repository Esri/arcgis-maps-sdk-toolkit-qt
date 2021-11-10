import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: groove

    property Item control
    property real offset
    property real progress
    property real visualProgress
    scale: control.horizontal && control.mirrored ? -1 : 1

    x: control.horizontal ? 0 : (control.availableWidth - width) / 2
    y: control.horizontal ? (control.availableHeight - height) / 2 : 0

    implicitWidth: control.horizontal ? 160 : 2
    implicitHeight: control.horizontal ? 2 : 160
    width: control.horizontal ? control.availableWidth : implicitWidth
    height: control.horizontal ? implicitHeight : control.availableHeight
    color: control.hovered ? Calcite.border1: Calcite.border2
}
