import QtQuick 2.12
import QtQuick.Templates 2.15 as T
import QtQuick.Controls 2.15

T.RangeSlider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            Math.max(first.implicitHandleWidth,
                                     second.implicitHandleWidth) + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             Math.max(first.implicitHandleHeight,
                                      second.implicitHandleHeight) + topPadding + bottomPadding)

    first.handle: SliderHandle {
        x: control.leftPadding + Math.round(control.horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + Math.round(control.horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height))

        palette: control.palette
        pressed: control.first.pressed
        hovered: control.hovered
        vertical: control.vertical
        visualFocus: visualFocus
        lingeringFocus: false
    }

    second.handle: SliderHandle {
        x: control.leftPadding + Math.round(control.horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + Math.round(control.horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))

        palette: control.palette
        pressed: control.second.pressed
        hovered: control.hovered
        vertical: control.vertical
        visualFocus: visualFocus
        lingeringFocus: false
    }

    background: SliderGroove {
        control: control
        offset: control.first.position
        progress: control.second.position
        visualProgress: control.second.visualPosition
    }
}
