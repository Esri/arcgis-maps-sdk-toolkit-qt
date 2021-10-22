import QtQuick 2.15

SequentialAnimation {
    id: anim
    property var target;
    property var delay: 0;
    PauseAnimation {
        duration: anim.delay
    }
    SequentialAnimation {
        loops: Animation.Infinite
        alwaysRunToEnd: true
        ParallelAnimation {
            PropertyAnimation {
                target: anim.target
                properties: "height"
                from:  rects.rectHeight
                to: rects.rectMaxHeight
                duration: 400
                easing.type: Easing.InSine
            }
            PropertyAnimation {
                target: anim.target
                properties: "opacity"
                from:  0.3
                to: 1.0
                duration: 400
                easing.type: Easing.InSine
            }
        }
        ParallelAnimation {
            PropertyAnimation {
                target: anim.target
                properties: "height"
                from: rects.rectMaxHeight
                to: rects.rectHeight
                duration: 400
                easing.type: Easing.OutSine
            }
            PropertyAnimation {
                target: anim.target
                properties: "opacity"
                from:  1.0
                to: 0.3
                duration: 400
                easing.type: Easing.OutSine
            }
        }
    }
}
