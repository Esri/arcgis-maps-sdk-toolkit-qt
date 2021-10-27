import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.RangeSlider {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
        Math.max(first.handle ? first.handle.implicitWidth : 0,
                 second.handle ? second.handle.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
        Math.max(first.handle ? first.handle.implicitHeight : 0,
                 second.handle ? second.handle.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 6

    first.handle: Rectangle {
        x: control.leftPadding + (horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height))
        implicitWidth: 28
        implicitHeight: 28
        radius: width / 2
        border.width: activeFocus ? 2 : 1
        border.color: control.enabled ? (activeFocus
            ? Default.focusColor
            : (control.first.pressed ? Default.indicatorFramePressedColor : Default.indicatorFrameColor)) : Default.indicatorFrameDisabledColor
        color: control.enabled ? (first.pressed
            ? (activeFocus ? Default.focusPressedColor : Default.indicatorPressedColor)
            : (activeFocus ? Default.focusLightColor : Default.backgroundColor)) : Default.indicatorDisabledColor

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }

    second.handle: Rectangle {
        x: control.leftPadding + (horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))
        implicitWidth: 28
        implicitHeight: 28
        radius: width / 2
        border.width: activeFocus ? 2 : 1
        border.color: control.enabled ? (activeFocus
            ? Default.focusColor
            : (control.first.pressed ? Default.indicatorFramePressedColor : Default.indicatorFrameColor)) : Default.indicatorFrameDisabledColor
        color: control.enabled ? (second.pressed
            ? (activeFocus ? Default.focusPressedColor : Default.indicatorPressedColor)
            : (activeFocus ? Default.focusLightColor : Default.backgroundColor)) : Default.indicatorDisabledColor

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }

    background: Rectangle {
        x: control.leftPadding + (horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? 200 : 6
        implicitHeight: horizontal ? 6 : 200
        width: horizontal ? control.availableWidth : implicitWidth
        height: horizontal ? implicitHeight : control.availableHeight
        radius: 3
        color: Default.buttonColor
        scale: horizontal && control.mirrored ? -1 : 1

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
}
