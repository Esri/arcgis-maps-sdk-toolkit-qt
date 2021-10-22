import QtQuick 2.15
import QtQuick.Templates 2.12 as T
import QtQuick.Layouts 1.12

T.BusyIndicator {
    id: control

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding

    spacing: 5

    contentItem: Item {
        id: rects
        implicitWidth: 3*(rectWidth + spacing)
        implicitHeight: rectMaxHeight

        readonly property double rectWidth: 13.6
        readonly property double rectHeight: 32
        readonly property double rectMaxHeight: 45

        Rectangle {
            id: rect1
            color: Calcite.brand
            width: rects.rectWidth
            height: rects.rectHeight
            anchors { 
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            BusyIndicatorAnim {
                id: anim1
                target: rect1
                running: control.visible && control.running
            }
        }

        Rectangle {
            id: rect2
            width: rects.rectWidth
            height: rects.rectHeight
            anchors {
              horizontalCenter: parent.horizontalCenter
              verticalCenter: parent.verticalCenter
            }
            color: Calcite.brand
            BusyIndicatorAnim {
                id: anim2
                target: rect2
                delay: 160
                running: control.visible && control.running
            }
        }

        Rectangle {
            id: rect3
            width: rects.rectWidth
            height: rects.rectHeight
            color: Calcite.brand
            anchors { 
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            BusyIndicatorAnim {
                id: anim3
                target: rect3
              delay: 320
                running: control.visible && control.running
            }
        }
    }
}
