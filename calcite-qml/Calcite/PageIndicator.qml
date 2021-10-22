import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.PageIndicator {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    delegate: Rectangle {
        implicitWidth: 5
        implicitHeight: 5

        radius: width / 2
        color: index === control.currentIndex ? Calcite.brand :
                                      pressed ? Calcite.brandPress : Calcite.foreground3
    }

    contentItem: Row {
        spacing: control.spacing
        Repeater {
            model: control.count
            delegate: control.delegate
        }
    }
}
