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

    icon.width: 78
    icon.height: 78

    // contentItem: Text {
    //     id: textItem
    //     text: control.text
    //     color: control.flat ? control.enabled ? Calcite.textLink : Calcite.text3 : Calcite.textInverse
    //     horizontalAlignment: Text.AlignHCenter
    //     verticalAlignment: Text.AlignVCenter
    //     elide: Text.ElideRight
    //     lineHeight: 1.5
    //     font: control.font
    //     Binding {
    //         target: textItem
    //         property: "font.bold"
    //         value: control.flat && control.checked ? true: false
    //     }
    // }

    contentItem: IconLabel {
        id: textItem
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.flat ? control.enabled ? Calcite.textLink : Calcite.text3 : Calcite.textInverse
        Binding {
            target: textItem
            property: "font.bold"
            value: control.flat && control.checked ? true: false
        }
    }

    background: Rectangle {
        radius: 0
        border {
            color: control.flat ? "transparent" : control.hovered ? Calcite.brandHover : Calcite.brand
            width: 1
        }

        height: control.flat ? control.checked ? 2 : 1 : undefined
        y: control.flat ? textItem.height : 0

        opacity: control.enabled ? 1.0 : 0.3
        color: {
         if (control.flat) {
            return control.hovered ? Calcite.textLink : "transparent"
         }  else {
            return control.pressed || control.Checked ? Calcite.brandPress
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
