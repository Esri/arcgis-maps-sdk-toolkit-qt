import QtQuick 2.12

Item {
    id: flashImage
    property alias running: animation.running
    property color color;
    opacity: 0

    signal finished();

    Rectangle {
      height: 16
      width: height
      radius: height
      color: flashImage.color
      x: -width/2
      y: -height/2
      border {
          color: "transparent"
          width: 1
      }
    }

    SequentialAnimation {
        id: animation
        running: true
        loops: 1
        NumberAnimation {
            target: flashImage
            property: "opacity"
            to: 1.0
            duration: 500
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: flashImage
            property: "opacity"
            to: 0.0
            duration: 500
            easing.type: Easing.InOutQuad
        }

        onFinished: {
            flashImage.finished()
            flashImage.destroy();
        }
    }
}