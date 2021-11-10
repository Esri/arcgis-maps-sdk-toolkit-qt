import QtQuick 2.15

Rectangle {
        property var control;
        width: 24
        height: 24
        border {
            width: 2
            color: control.checkState !== Qt.Unchecked ? control.hovered ? Calcite.brandHover : Calcite.brand :
            control.hovered ? Calcite.text3 : Calcite.borderInput
        }
        color: control.checkState === Qt.Unchecked ? Calcite.offWhite : control.hovered ? Calcite.brandHover : Calcite.brand
        radius: 1
        opacity: enabled ? 1.0 : 0.3

        Text {
                anchors.fill: parent
                font {
                    pixelSize: 16
                    weight: Font.Bold
                }
                color: Calcite.offWhite
                visible: control.checkState !== Qt.Unchecked
                text: control.checkState === Qt.Checked ? "\u2713" : "\u2012"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
    }
