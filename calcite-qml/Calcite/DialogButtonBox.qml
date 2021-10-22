import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.DialogButtonBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    spacing: 5
    padding: 16
    buttonLayout: DialogButtonBox.MacLayout
    delegate: Button { }

    contentItem: ListView {
          implicitWidth: parent.width
          implicitHeight: 40
          model: control.contentModel
          spacing: control.spacing
          orientation: ListView.Horizontal
          boundsBehavior: Flickable.StopAtBounds
          snapMode: ListView.SnapToItem
      }

        background: Rectangle {
            border {
                width: 1
                color: Calcite.border1
            }
            color: Calcite.background
        }
}
