import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T
import QtGraphicalEffects 1.12

T.ItemDelegate {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 5
    spacing: 5

    icon.width: 16
    icon.height: 16

    property color color : control.enabled ? control.hovered || control.down ? Calcite.offWhite : Calcite.text1 : Calcite.text3

    indicator: Image {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        visible: control.highlighted
        source: "images/check.svg"
        ColorOverlay {
            anchors.fill: parent
            source: parent
            color: control.color
        }
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        leftPadding: {
            if (!control.mirrored && control.indicator) {
                const width = control.indicator.width;
                if (width > 0) {
                    return control.spacing + width;
                }
            }
            return 0;
        }
        rightPadding: {
            if (control.mirrored && control.indicator) {
                const width = control.indicator.width;
                if (width > 0) {
                    return control.spacing + width;
                }
            }
            return 0;
        }
        alignment: control.display === IconLabel.IconOnly || control.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft

        icon: control.icon
        text: control.text
        font: control.font
        color: control.color
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.visualFocus || control.down ? Calcite.brandPress : control.hovered ? Calcite.brand : "transparent"
    }
}
