import QtQuick 2.12


Rectangle {
        property var control;

        implicitWidth: 20
        implicitHeight: 20
        border {
            width: 2
            color: control.checked ? control.hovered ? Calcite.brandHover : Calcite.brand : control.hovered ? Calcite.text3 : Calcite.borderInput
        }
        radius: width / 2
        color: Calcite.offWhite
        opacity: enabled ? 1 : 0.3

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 12
            height: 12
            radius: width / 2
            color: control.hovered ? Calcite.brandHover : Calcite.brand
            visible: control.checked
        }
    }