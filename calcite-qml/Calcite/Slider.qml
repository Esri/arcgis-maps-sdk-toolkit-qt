import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.Slider {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            (handle ? handle.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             (handle ? handle.implicitHeight : 0) + topPadding + bottomPadding)

    handle: SliderHandle {
        id: handle
        x: control.leftPadding + Math.round(control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + Math.round(control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))

        palette: control.palette
        pressed: control.pressed
        hovered: control.hovered
        vertical: control.vertical
        visualFocus: control.visualFocus
        lingeringFocus: false // control.activeFocus
    }

    background: SliderGroove {
        control: control
        progress: control.position
        visualProgress: control.visualPosition
    }
}
