import QtQuick 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Templates 2.15 as T

T.Button {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    topPadding: 6
    bottomPadding: 6
    leftPadding: 16
    rightPadding: 16
    spacing: 6

    icon.width: 24
    icon.height: 24

    property color color: control.flat ? control.enabled ? Calcite.textLink : Calcite.text3 : Calcite.textInverse
    icon.color: color

    contentItem: IconLabel {
        id: textItem
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.color
        Binding {
            target: textItem
            property: "font.bold"
            value: control.flat && control.checked ? true: false
        }
    }

    background: Rectangle {
        radius: 0
        implicitWidth: 80
        implicitHeight: 24
        border {
            color: control.flat ? "transparent" : control.hovered ? Calcite.brandHover : Calcite.brand
            width: control.flat ? 0 : 1
        }

        height: control.flat ? control.checked ? 2 : 1 : undefined
        x: control.flat? textItem.x : 0
        y: control.flat ? control.baselineOffset + height : 0
        width: control.flat ? textItem.width: undefined

        opacity: control.enabled ? 1.0 : 0.3
        color: {
            if (control.flat) {
                return control.hovered || control.checked ? Calcite.textLink : "transparent"
            }  else {
                return control.pressed || control.checked ? Calcite.brandPress
                                                          : control.hovered ? Calcite.brandHover
                                                                            : Calcite.brand
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: 50
            }
        }
    }
}
