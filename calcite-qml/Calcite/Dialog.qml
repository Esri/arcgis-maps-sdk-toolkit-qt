import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.Dialog {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            header && header.visible ? header.implicitWidth : 0,
                            footer && footer.visible ? footer.implicitWidth : 0,
                            contentWidth > 0 ? contentWidth + leftPadding + rightPadding : 0)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             (header && header.visible ? header.implicitHeight + spacing : 0)
                             + (footer && footer.visible ? footer.implicitHeight + spacing : 0)
                             + (contentHeight > 0 ? contentHeight + topPadding + bottomPadding : 0))

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: 16

    background: Rectangle {
        border {
            width: 1
            color: Calcite.border1
        }
        color: Calcite.background
    }

    header: Label {
        text: control.title
        visible: control.title
        elide: Label.ElideRight
        padding: 16
        font.pixelSize: control.font.pixelSize + 10
        background: Rectangle {
            border {
                width: 1
                color: Calcite.border1
            }
            color: Calcite.background
        }
    }

    footer: DialogButtonBox {
        visible: count > 0
    }
}
