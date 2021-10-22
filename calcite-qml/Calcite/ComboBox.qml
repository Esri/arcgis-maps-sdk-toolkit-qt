import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Templates 2.12 as T

T.ComboBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    delegate: ItemDelegate {
        width: control.popup.width
        text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        highlighted: control.currentIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: Text {
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        rightPadding: 5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "\u2195"
        font {
            weight: Font.Black
            pixelSize: 22
            bold: true
        }
        color: Calcite.borderInput
    }

    contentItem: Text {
        text: control.displayText
        font: control.font
        leftPadding: 10
        rightPadding: 24
        topPadding: 0
        bottomPadding: 0
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        opacity: enabled ? 1.0 : 0.3
        color: Calcite.text1
        lineHeightMode: Text.FixedHeight
        lineHeight: 36
    }

    background: Rectangle {
        radius: 0
        implicitWidth: 120
        implicitHeight: 32
        border {
            width: 1
            color: control.activeFocus ? Calcite.brand : Calcite.borderInput
            Behavior on color {
              ColorAnimation {
                  duration: 50
              }
          }
        }
        color: Calcite.background
        visible: !control.flat || control.pressed || control.hovered || control.visualFocus
    }

    popup: T.Popup {
        width: control.width
        implicitHeight: Math.min(396, contentItem.implicitHeight)
        topMargin: 8
        bottomMargin: 8

        contentItem: ListView {
            id: list
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            highlightRangeMode: ListView.ApplyRange
            T.ScrollIndicator.vertical: ScrollIndicator { }
            highlight: Rectangle {
                width: list.currentItem ? list.currentItem.width : 0
                height: list.currentItem ? list.currentItem.height : 0
                color: !list.currentItem ? "transparent"
                                         : list.currentItem.pressed ? Calcite.brandPress
                                                                    : list.currentItem.hovered ? Calcite.brand
                                                                                               : "transparent"
            }
        }

        background: Rectangle {
            color: Calcite.background
            border {
                color: Calcite.border1
                width: Calcite.theme === Calcite.Light ?  1 : 0
            }
        }
    }
}
