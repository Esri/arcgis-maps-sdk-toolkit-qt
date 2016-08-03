import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISExtras 1.1
import "LeaderPosition.js" as Enums

Item {
    id: root
    x: 0
    y: 0

    visible: false

    // configurable properties
    property bool autoAdjustWidth: true
    property var leaderPosition: Enums.LeaderPosition.Bottom
    property color borderColor: "#000000"
    property int borderWidth: 2
    property color backgroundColor: "#AFFFFFFF"
    property color titleColor: "#000000"
    property color detailColor: "#000000"
    property int cornerRadius: 10
    property int leaderHeight: 15
    property int leaderWidth: 30
    property int screenOffsetx: 0
    property int screenOffsety: 0
    property var calloutData

    // internal properties
    property int padding: 3
    property real scaleFactor: System.displayScaleFactor
    property real anchorPointx: 0
    property real anchorPointy: 0
    property point screenCoordinates
    property var adjustedLeaderPosition: Enums.LeaderPosition.Bottom
    property bool hidden: true
    property real calloutMaxWidth: 210
    property real calloutMaxHeight: 100
    property real calloutMinWidth: calloutMaxWidth
    property real calloutMinHeight: calloutMaxHeight
    property real rectWidth: 0
    property real rectHeight: 0
    property real edgeBuffer: 10
    property real calloutFramePadding: 2 * cornerRadius
    property string platform: Qt.platform.os
    property real halfRectWidth: rectWidth / 2
    property real halfRectHeight: rectHeight / 2
    property real halfLeaderWidth: leaderWidth / 2
    property bool debug: false

    Connections {
        id: calloutConnection
        target: calloutData
        onScreenPointChanged: {
            anchorPointx = calloutData.screenPoint.x;
            anchorPointy = calloutData.screenPoint.y;

            // add any configured offsets
            if (screenOffsetx !== 0)
                anchorPointx += screenOffsetx
            if (screenOffsety !== 0)
                anchorPointy += screenOffsety

            if (!hidden)
                showCallout();
        }
    }

    function showCallout() {
        hidden = false;
        root.visible = true;

        if (leaderPosition !== Enums.LeaderPosition.Automatic)
            adjustedLeaderPosition = leaderPosition

        // these are some of the initial calculations
        // before creating the callout frame
        preCalculateWidthAndHeight();
        adjustRelativePositionOfCanvasFrame(anchorPointx, anchorPointy);

        if (leaderPosition !== Enums.LeaderPosition.Automatic)
            adjustedLeaderPosition = leaderPosition;

        // create the callout frame don't paint yet.
        canvas.createPathAndPaint = false;
        canvas.requestPaint();

        // once leader position is finalized
        if (findBestLeaderPosition(anchorPointx, anchorPointy)) {
            adjustRelativePositionOfCanvasFrame(anchorPointx, anchorPointy);

        }

        // paint now.
        canvas.createPathAndPaint = true;
        canvas.requestPaint(true);
        calloutFrame.visible = true;
    }

    function dismiss() {
        hidden = true;
        calloutFrame.visible = false;
    }

    Rectangle {
        id: calloutFrame
        width: (rectWidth + 2* leaderWidth + edgeBuffer) * scaleFactor
        height: (rectHeight + 2* leaderHeight + edgeBuffer) * scaleFactor
        visible: false
        z: 100
        clip:true
        color: "transparent"

        Canvas {
            id: canvas
            anchors.fill: parent

            property bool createPathAndPaint: false

            antialiasing: true

            renderTarget: Canvas.FramebufferObject
            renderStrategy: Canvas.Cooperative

            // handler to override for drawing
            onPaint: {
                screenCoordinates.x = 0;
                screenCoordinates.y = 0;
                drawCalloutFrame();
            }

            Rectangle {
                id: calloutContentFrame

                anchors {
                    left: parent.left
                    top: parent.top
                    topMargin: leaderHeight * scaleFactor
                    leftMargin: 7
                }

                GridLayout {
                    id: calloutLayout
                    columns: 3
                    rows: 2
                    anchors {
                        left: parent.left
                        top: parent.top
                   }
                    columnSpacing: 7 * scaleFactor

                    Image {
                        id: image
                        source: calloutData.imageUrl
                        fillMode : Image.PreserveAspectFit
                        Layout.rowSpan: 2
                    }

                    Text {
                        id: title
                        text: calloutData.title
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        font.pixelSize: 20 * scaleFactor
                    }

                    Rectangle {
                        width: 35 * scaleFactor
                        height: width
                        Layout.rowSpan: 2
                        color: "transparent"
                        border {
                            color: "navy"
                            width: 2
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "i"
                            color: "navy"
                            font.pixelSize: 20 * scaleFactor
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Info clicked");
                            }
                        }
                    }

                    Text {
                        id: detail
                        text: calloutData.detail
                        font.pixelSize: 15 * scaleFactor
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }
        }
    }

    // Draw the rounded rectangle with leader
    function drawCalloutFrame() {

        // Adjust the relative coordinates only for Top and Left to account
        // for leaderHeight
        if (adjustedLeaderPosition === Enums.LeaderPosition.Top || adjustedLeaderPosition === Enums.LeaderPosition.UpperRight || adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            screenCoordinates.y += leaderHeight;
        }
        if (adjustedLeaderPosition === Enums.LeaderPosition.Left)
            screenCoordinates.x += leaderHeight;

        // get context to draw with
        var ctx = canvas.getContext("2d");

        // setup the style
        ctx.lineWidth = borderWidth;
        ctx.strokeStyle = "darkGray";
        ctx.fillStyle = "white";
        ctx.alpha = 1.0;

        ctx.save();
        ctx.clearRect(0,0,canvas.width, canvas.height);
        ctx.lineJoin = "round";

        ctx.beginPath();

        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            ctx.moveTo(screenCoordinates.x + leaderWidth,screenCoordinates.y);
        } else {
            ctx.moveTo(screenCoordinates.x + cornerRadius,screenCoordinates.y);
        }

        // top side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Top) {
            ctx.lineTo(screenCoordinates.x + (halfRectWidth - halfLeaderWidth), screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + halfRectWidth, screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + (halfRectWidth + halfLeaderWidth), screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + rectWidth - cornerRadius, screenCoordinates.y);
        } else {
            if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
                ctx.lineTo(screenCoordinates.x + rectWidth-leaderWidth,screenCoordinates.y);

            } else {
                ctx.lineTo(screenCoordinates.x + rectWidth-cornerRadius,screenCoordinates.y);

            }
        }

        // draw top right corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
            ctx.lineTo(screenCoordinates.x + (rectWidth - halfLeaderWidth), screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth,screenCoordinates.y);
        } else {
            ctx.arcTo(screenCoordinates.x + rectWidth,screenCoordinates.y,screenCoordinates.x+rectWidth,screenCoordinates.y+cornerRadius,cornerRadius);
        }

        // right side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Right) {
            ctx.lineTo(screenCoordinates.x+rectWidth , screenCoordinates.y + cornerRadius + halfRectHeight - halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x+rectWidth + leaderHeight, screenCoordinates.y + halfRectHeight);
            ctx.lineTo(screenCoordinates.x+rectWidth, screenCoordinates.y + halfRectHeight + halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x + rectWidth, screenCoordinates.y+rectHeight-cornerRadius);
        } else {
            ctx.lineTo(screenCoordinates.x+rectWidth,screenCoordinates.y+rectHeight-cornerRadius);
        }
        // draw bottom right corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.LowerRight) {
            ctx.lineTo(screenCoordinates.x + rectWidth - halfLeaderWidth, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth - leaderWidth ,screenCoordinates.y + rectHeight );
        } else {
            ctx.arcTo(screenCoordinates.x+rectWidth,screenCoordinates.y+rectHeight,screenCoordinates.x+rectWidth-cornerRadius,screenCoordinates.y+rectHeight,cornerRadius);
        }

        // bottom side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Bottom) {
            ctx.lineTo(screenCoordinates.x+halfRectWidth+halfLeaderWidth, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x+halfRectWidth, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x+halfRectWidth-halfLeaderWidth, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + cornerRadius, screenCoordinates.y+rectHeight);
        } else {
            if (adjustedLeaderPosition === Enums.LeaderPosition.LowerLeft) {
                ctx.lineTo(screenCoordinates.x+leaderWidth,screenCoordinates.y+rectHeight);

            } else {
                ctx.lineTo(screenCoordinates.x+cornerRadius,screenCoordinates.y+rectHeight);
            }
        }

        // draw bottom left corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.LowerLeft) {
            ctx.lineTo(screenCoordinates.x + halfLeaderWidth, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + rectHeight);
        } else {
            ctx.arcTo(screenCoordinates.x,screenCoordinates.y+rectHeight,screenCoordinates.x,screenCoordinates.y+rectHeight-cornerRadius,cornerRadius);
        }

        // left side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Left) {
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + halfRectHeight + halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x - leaderHeight, screenCoordinates.y + halfRectHeight);
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + halfRectHeight - halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y+cornerRadius);
        } else {
            ctx.lineTo(screenCoordinates.x,screenCoordinates.y+cornerRadius);
        }

        // draw top left corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            ctx.lineTo(screenCoordinates.x + halfLeaderWidth , screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + leaderWidth,screenCoordinates.y);
        } else {
            ctx.arcTo(screenCoordinates.x,screenCoordinates.y,screenCoordinates.x+cornerRadius,screenCoordinates.y,cornerRadius);
        }
        ctx.closePath();

        if (canvas.createPathAndPaint) {
            ctx.fill();
            ctx.stroke();
        }

        ctx.restore();

    }

    // Changes the actual leader position (stored in adjustedLeaderPosition) if the current style specifies
    // Enums.LeaderPosition.Automatic and the current actual leader position isn't optimal for the current size and position of
    // the callout.
    function findBestLeaderPosition(mousex, mousey) {

        var refresh = false;

        if (debug) {
            console.log("mousex = ", mousex);
            console.log("mousey = ", mousey);
            console.log("calloutFrame.width = ", calloutFrame.width);
            console.log("calloutFrame.height = ", calloutFrame.height);
            console.log("root.parent.width = ", root.parent.width);
            console.log("root.parent.height = ", root.parent.height);
        }

        if (leaderPosition === Enums.LeaderPosition.Automatic) {

            // Move leader horizontally if horizontal position isn't optimal
            if (mousex + calloutFrame.width > root.parent.width) {
                // Right edge of callout is right of right edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Right, mousex, mousey);
            } else if (mousex - calloutFrame.width < 0) {
                // Left edge of callout is left of left edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Left, mousex, mousey);
            }

            // Move leader vertically if vertical position isn't optimal
            if (mousey + calloutFrame.height > root.parent.height && !refresh) {
                // Bottom edge of callout is below bottom edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Down, mousex, mousey);
            } else if (mousey - calloutFrame.height < 0 && !refresh) {
                // Top edge of callout is above top edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Up, mousex, mousey);
            }
        }

        return refresh;
    }

    // Moves the leader position following one of 4 directions: Down, Up, Left or Right.
    // Return true if a new leaderPosition has been set.
    function moveLeader(direction, mousex, mousey) {
        var refresh = false;
        var mapViewWidth = root.parent.width;
        var mapViewHeight = root.parent.height;
        var anchorX = mousex;
        var anchorY = mousey;

        // Callout is 'narrow' if it's less than half the width of the MapView
        var narrowCallout = true;
        if (calloutFrame.width > mapViewWidth / 2) {
          narrowCallout = false;
        }

        // Callout is 'short' if it's less than half the height of the MapView
        var shortCallout = true;
        if (calloutFrame.height > mapViewHeight / 2) {
          shortCallout = false;
        }

        switch (direction) {
          case Enums.LeaderMoveDirection.Down:
            // Bottom edge of callout is below bottom edge of map.
            // Move leader down so the callout moves up
            switch (adjustedLeaderPosition) {
              case Enums.LeaderPosition.UpperLeft:
                if (shortCallout || anchorY > mapViewHeight / 3) {
                  adjustedLeaderPosition = Enums.LeaderPosition.Left;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Left:
                if (shortCallout || anchorY > mapViewHeight * 2 / 3) {
                  adjustedLeaderPosition = Enums.LeaderPosition.LowerLeft;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Top:
                if (shortCallout || anchorY > mapViewHeight / 2) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Bottom;
                    refresh = true;
                }
                break;
              case Enums.LeaderPosition.UpperRight:
                if (shortCallout || anchorY > mapViewHeight / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Right;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Right:
                if (shortCallout || anchorY > mapViewHeight * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.LowerRight;
                  refresh = true;
                }
                break;
            }
            break;
          case Enums.LeaderMoveDirection.Up:
            // Top edge of callout is above top edge of map.
            // Move leader up so the callout moves down
            switch (adjustedLeaderPosition) {
              case Enums.LeaderPosition.LowerLeft:
                if (shortCallout || anchorY < mapViewHeight * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Left
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Left:
                if (shortCallout || anchorY < mapViewHeight / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.UpperLeft;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Bottom:
                if (shortCallout || anchorY < mapViewHeight / 2) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Top;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.LowerRight:
                if (shortCallout || anchorY < mapViewHeight * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Right;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Right:
                if (shortCallout || anchorY < mapViewHeight / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.UpperRight;
                  refresh = true;
                }
                break;
            }
            break;
          case Enums.LeaderMoveDirection.Left:
            // Left edge of callout is left of left edge of map.
            // Move leader to the left so the callout moves right
            switch (adjustedLeaderPosition) {
              case Enums.LeaderPosition.UpperRight:
                if (narrowCallout || anchorX < mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Top;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Right:
                if (narrowCallout || anchorX < mapViewWidth / 2) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Left;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.LowerRight:
                if (narrowCallout || anchorX < mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Bottom;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Top:
                if (narrowCallout || anchorX < mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.UpperLeft;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Bottom:
                if (narrowCallout || anchorX < mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.LowerLeft;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.LowerLeft:
                if (narrowCallout || anchorX < mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Left;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.UpperLeft:
                if (narrowCallout || anchorX < mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Left;
                  refresh = true;
                }
                break;
            }
            break;
          case Enums.LeaderMoveDirection.Right:
            // Right edge of callout is right of right edge of map.
            // Move leader to the right so the callout moves left
            switch (adjustedLeaderPosition) {
              case Enums.LeaderPosition.UpperLeft:
                if (narrowCallout || anchorX > mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Top;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Left:
                if (narrowCallout || anchorX > mapViewWidth / 2) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Right;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.LowerLeft:
                if (narrowCallout || anchorX > mapViewWidth / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Bottom;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Top:
                if (narrowCallout || anchorX > mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.UpperRight;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.Bottom:
                if (narrowCallout || anchorX > mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.LowerRight;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.LowerRight:
                if (narrowCallout || anchorX > mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Right;
                  refresh = true;
                }
                break;
              case Enums.LeaderPosition.UpperRight:
                if (narrowCallout || anchorX > mapViewWidth * 2 / 3) {
                    adjustedLeaderPosition = Enums.LeaderPosition.Right;
                  refresh = true;
                }
                break;
            }
            break;
        }
        return refresh;
    }

    function preCalculateWidthAndHeight() {

        // Calculate width and height of the rectangle with curved corners that we're going to draw.
        // This is the basic shape of the callout, but excluding the leader and not yet including extra room to take account
        // of the border having width.

        var maxWidth = calloutContentMaxWidth() + 2*cornerRadius;
        var maxHeight = calloutContentMaxHeight() + 2*cornerRadius;
        var minWidth = Math.min(calloutMinWidth, maxWidth); // don't allow minWidth to be > maxWidth
        var minHeight = Math.min(calloutMinHeight, maxHeight); // don't allow minHeight to be > maxHeight

        if (title.text.length > 0 && detail.text.length > 0) {
            // If we know the width of the content, base the width on that
            if (calloutLayout.width === 0) {
                rectWidth = minWidth;
            } else {
                if (platform === "ios") {
                    rectWidth = calloutLayout.width + calloutFramePadding + 2* leaderWidth
                } else {
                    rectWidth = calloutLayout.width + calloutFramePadding;
                }
            }

            // If we know the height of the content, base the height on that
            if (calloutLayout.height === 0) {
                rectHeight = minHeight;
            } else {
                rectHeight = calloutLayout.height + calloutFramePadding + leaderHeight;
            }
        } else {
            rectWidth = minWidth;
            rectHeight = minHeight;
        }

        if (debug) {
            console.log("rectWidth = ", rectWidth);
            console.log("rectHeight = ", rectHeight);
            console.log("calloutLayout.width = ", calloutLayout.width);
            console.log("calloutLayout.height = ", calloutLayout.height);
            console.log("calloutContentFrame.width = ", calloutContentFrame.width);
        }
    }

    function adjustRelativePositionOfCanvasFrame(screenx, screeny) {

        if (adjustedLeaderPosition === Enums.LeaderPosition.Top ) {
            calloutFrame.x = screenx - rectWidth / 2;
            calloutFrame.y = screeny;
            if (debug) {
                console.log("top calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Bottom) {
            calloutFrame.x = screenx - (rectWidth / 2);
            calloutFrame.y = screeny  - (leaderHeight + rectHeight );
            if (debug) {
                console.log("bottom calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Left) {
            calloutContentFrame.anchors.leftMargin = leaderHeight;
            calloutFrame.x = screenx;
            calloutFrame.y = screeny  - rectHeight / 2;
            if (debug) {
                console.log("left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Right) {
            calloutFrame.x = screenx - (rectWidth + leaderWidth);
            calloutFrame.y = screeny  - rectHeight / 2;
            if (debug) {
                console.log("right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
            calloutFrame.x = screenx - leaderWidth / 2 - (rectWidth - leaderWidth);
            calloutFrame.y = screeny;
            if (debug) {
                console.log("upper right top right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            calloutFrame.x = screenx - leaderWidth / 2;
            calloutFrame.y = screeny;
            if (debug) {
                console.log("upper left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.LowerRight) {
            calloutFrame.x = screenx - rectWidth + leaderWidth / 2;
            calloutFrame.y = screeny  - (leaderHeight + rectHeight);
            if (debug) {
                console.log("lower right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.LowerLeft) {
            calloutFrame.x = screenx - leaderWidth / 2;
            calloutFrame.y = screeny  - (leaderHeight + rectHeight);
            if (debug) {
                console.log("lower left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        }

    }

    function calloutContentMaxWidth() {
        // Start by getting width of MapView
        var maxWidthForMapView = root.parent.width;

        // Calculate space to allow for 2 corners and 1 border width.
        var widthOfExtras = borderWidth + (2 * cornerRadius);

        // Allow space for the leader, plus 2 extra leader lengths to ensure there's always room for some map to show around
        // the outside of the callout.
        widthOfExtras += (3 * leaderHeight);
        maxWidthForMapView -= widthOfExtras;

        // If max width has been specified and fits in MapView, just return that value
        var styleMaxWidthExclExtras = calloutMaxWidth - widthOfExtras;
        if (styleMaxWidthExclExtras > 0 && styleMaxWidthExclExtras < maxWidthForMapView) {
            return styleMaxWidthExclExtras;
        }

        // Otherwise return max width that fits in MapView
        return maxWidthForMapView;
    }

    function calloutContentMaxHeight() {
        // Start by getting height of MapView
        var maxHeightForMapView = root.parent.height;

        // Calculate space to allow for 2 corners and 1 border width.
        var heightOfExtras = borderWidth + (2 * cornerRadius);

        // Allow space for the leader, plus 2 extra leader lengths to ensure there's always room for some map to show around
        // the outside of the callout.
        heightOfExtras += (3 * leaderHeight);
        maxHeightForMapView -= heightOfExtras;

        // If max height has been specified and fits in MapView, just return that value
        var styleMaxHeightExclExtras = calloutMaxHeight - heightOfExtras;
        if (styleMaxHeightExclExtras > 0 && styleMaxHeightExclExtras < maxHeightForMapView) {
            return styleMaxHeightExclExtras;
        }

        // Otherwise return max height that fits in MapView
        return maxHeightForMapView;
    }
}
