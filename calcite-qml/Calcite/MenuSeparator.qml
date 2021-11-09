import QtQuick 2.12
import QtQuick.Templates 2.15 as T

T.MenuSeparator {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 2
    topPadding: padding + 4
    bottomPadding: padding + 4

    contentItem: Rectangle {
        implicitWidth: 188
        implicitHeight: 1
        color: Calcite.border1
    }
}
