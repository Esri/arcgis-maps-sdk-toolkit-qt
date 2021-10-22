import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.ProgressBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: Rectangle {
        id: strip
        implicitHeight: 2

        scale: control.mirrored ? -1 : 1
        color: Calcite.brand
        clip: control.indeterminate
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 2

        x: control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: control.availableWidth
        height: 2

        visible: !control.indeterminate
        color: Calcite.foreground2
    }
}
