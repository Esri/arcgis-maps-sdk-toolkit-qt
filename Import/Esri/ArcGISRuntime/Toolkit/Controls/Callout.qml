/*******************************************************************************
 *  Copyright 2012-2018 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
 
import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import "LeaderPosition.js" as Enums

/*!
    \qmltype Callout
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.1
    \brief A view for displaying information at a geographic location on a Map.

     A Callout can be displayed for several different scenarios:

     \list
        \li To display the coordinates where a user tapped on the map.
        \li To display information about a GeoElement that has been identified
        on the MapView.
        \li To display a callout at your current location.
     \endlist

     For more information, please see the CalloutData documentation.
*/
Item {
    id: root
    x: 0
    y: 0    

    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief Whether to automatically adjust the width of the Callout based on content.

        The default is \c true.
    */
    property bool autoAdjustWidth: true

    /*!
        \brief The enumerator for the set of possible positions of the leader line in the callout.

        LeaderPosition includes:

        \list
            \li leaderPositionEnum.UpperLeft (0)
            \li leaderPositionEnum.Top (1)
            \li leaderPositionEnum.UpperRight (2)
            \li leaderPositionEnum.Right (3)
            \li leaderPositionEnum.LowerRight (4)
            \li leaderPositionEnum.Bottom (5)
            \li leaderPositionEnum.LowerLeft (6)
            \li leaderPositionEnum.Left (7)
            \li leaderPositionEnum.Automatic (8)
        \endlist

        Automatic will decide the best placement, based on the
        location of the callout within the visible area of the MapView.

        The default is \c leaderPositionEnum.Bottom.
    */
    property var leaderPositionEnum: Enums.LeaderPosition

    /*!
        \brief The property to set the leader position of the callout.

        For example, to set the leader line to the top of the callout use:

        \c Callout.leaderPosition: leaderPositionEnum.Top
    */
    property var leaderPosition: leaderPositionEnum.Bottom

    /*!
        \brief The border color of the Callout.

        The default color is \c #000000.
    */
    property color borderColor: "#000000"

    /*!
        \brief The border width of the Callout.

        The default width is \c 2.
    */
    property int borderWidth: 2 * displayScaleFactor

    /*!
        \brief The background color of the Callout.

        The default color is \c "#ffffff".
    */
    property color backgroundColor: "#ffffff"

    /*!
        \brief The color of the title text in the Callout.

        The default color is \c #000000.
    */
    property color titleTextColor: "#000000"

    /*!
        \brief The color of the detail text in the Callout.

        The default color is \c #000000.
    */
    property color detailTextColor: "#000000"

    /*!
        \brief The corner radius of the Callout.

        The default value is \c 5.
    */
    property int cornerRadius: 5 * displayScaleFactor

    /*!
        \brief The height of the leader line in the Callout.

        The default leader height is \c 10.
    */
    property int leaderHeight: 10 * displayScaleFactor

    /*!
        \brief The width of the leader line in the Callout.

        The default leader width is \c 20.
    */
    property int leaderWidth: 20 * displayScaleFactor

    /*!
        \brief The x offset of the placement of the Callout.

        The default is \c 0.
    */
    property int screenOffsetX: 0 * displayScaleFactor

    /*!
        \brief The y offset of the placement of the Callout.

        The default is \c 0.
    */
    property int screenOffsetY: 0 * displayScaleFactor

    /*!
        \brief The type of accessory button to be displayed in the Callout.

        Default is "Info".

        \list
            \li "Info"
            \li "Add"
            \li "Custom"
        \endlist
    */
    property string accessoryButtonType: "Info"

    /*!
        \brief The url of the image to be used for the accessory button of the Callout if the type
        of the accessoryButton is "Custom".
    */
    property string customImageUrl

    /*!
        \brief Whether to hide the accessoryButton of the Callout.

        The default is \c false.
    */
    property bool accessoryButtonHidden: false

    /*!
        \brief The signal emitted when the accessory button is clicked.
    */
    signal accessoryButtonClicked()

    /*!
        \brief The CalloutData to display in the Callout.

        The CalloutData controls the data that is being displayed
        in the Callout view. Use CalloutData to set title text,
        detail text, images, GeoElements, and so on.
        CalloutData is obtained from the MapView.
    */
    property var calloutData: null

    /*!
        \brief The maximum width of the Callout if autoAdjustWidth is true.
    */
    property real maxWidth: 300 * displayScaleFactor

    /*!
        \brief The width of the Callout if autoAdjustWidth is false.
    */
    property real calloutWidth: 300 * displayScaleFactor

    // internal properties
    /*! \internal */
    property int padding: 3 * displayScaleFactor
    /*! \internal */
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" || Qt.platform.os === "linux" ? 96 : 72)
    /*! \internal */
    property real anchorPointx: 0
    /*! \internal */
    property real anchorPointy: 0
    /*! \internal */
    property point screenCoordinates
    /*! \internal */
    property var adjustedLeaderPosition: Enums.LeaderPosition.Bottom
    /*! \internal */
    property bool calloutVisible
    /*! \internal */
    property real calloutMaxHeight: 45 * displayScaleFactor
    /*! \internal */
    property real calloutMinWidth: 95 * displayScaleFactor
    /*! \internal */
    property real calloutMinHeight: calloutMaxHeight
    /*! \internal */
    property real rectWidth: 0
    /*! \internal */
    property real rectHeight: 0
    /*! \internal */
    property real edgeBuffer: 10 * displayScaleFactor
    /*! \internal */
    property real calloutFramePadding: (2 * internalCornerRadius) * displayScaleFactor
    /*! \internal */
    property string platform: Qt.platform.os
    /*! \internal */
    property real halfRectWidth: rectWidth / 2
    /*! \internal */
    property real halfRectHeight: rectHeight / 2
    /*! \internal */
    property real halfLeaderWidth: leaderWidth / 2
    /*! \internal */
    property real imageWidth: rectWidth / 4
    /*! \internal */
    property real titleWidth: Math.max(0, rectWidth - 95 * displayScaleFactor)
    /*! \internal */
    property real detailWidth: Math.max(0, rectWidth - 95 * displayScaleFactor)
    /*! \internal */
    property real cornerOffset: 15 * displayScaleFactor
    /*! \internal */
    property bool debug: false
    /*! \internal */
    property int internalCornerRadius: cornerRadius < 0 ? 0 : cornerRadius
    /*! \internal */
    property int adjustedMaxWidth: maxWidth
    /*! \internal */
    visible: false

    /*! \internal */
    Connections {
        id: calloutConnection
        target: calloutData
        onScreenPointChanged: {
            anchorPointx = calloutData.screenPoint.x;
            anchorPointy = calloutData.screenPoint.y;

            // add any configured offsets
            if (screenOffsetX !== 0)
                anchorPointx += screenOffsetX
            if (screenOffsetY !== 0)
                anchorPointy += screenOffsetY
            if (calloutVisible)
                showCallout();
        }

        onVisibleChanged: {
            if (calloutData.visible) {
                showCallout();
            }
            else {
                dismiss();
            }
        }

        onTitleChanged: {
            if (calloutVisible)
                showCallout();
        }
    }

    onLeaderPositionChanged: {
        if (calloutVisible)
            showCallout();
    }

    /*!
        \brief Show the Callout on the MapView.

        Before showing the callout, set your desired properties for
        CalloutData (which controls the information that is displayed)
        and for Callout (which controls how the view appears on the MapView).
    */
    function showCallout() {

        // no calloutData set
        if (!calloutData)
            return;

        calloutVisible = true;
        root.visible = true;

        // set the adjustedLeaderPosition
        if (leaderPosition !== Enums.LeaderPosition.Automatic)
            adjustedLeaderPosition = leaderPosition;
        else
            adjustedLeaderPosition = Enums.LeaderPosition.Bottom;

        // setup the accessory button mode
        setupAccessoryButton();

        // these are some of the initial calculations
        // before creating the callout frame
        preCalculateWidthAndHeight();
        adjustRelativePositionOfCanvasFrame(anchorPointx, anchorPointy, rectWidth, rectHeight);

        if (leaderPosition !== Enums.LeaderPosition.Automatic)
            adjustedLeaderPosition = leaderPosition;

        // create the callout frame don't paint yet.
        canvas.createPathAndPaint = false;
        canvas.requestPaint();

        // once leader position is finalized
        if (findBestLeaderPosition(anchorPointx, anchorPointy)) {
            adjustRelativePositionOfCanvasFrame(anchorPointx, anchorPointy, rectWidth, rectHeight);
        }

        // paint now.
        canvas.createPathAndPaint = true;
        canvas.requestPaint(true);
        calloutFrame.visible = true;
    }

    /*!
        \brief Dismisses the Callout from the MapView.

        The Callout does not hide itself automatically, so you must
        explicitly call this method to hide the Callout from the
        MapView.
    */
    function dismiss() {
        calloutVisible = false;
        calloutFrame.visible = false;
    }

    Rectangle {
        id: calloutFrame
        width: (rectWidth + 2 * leaderWidth + edgeBuffer) * displayScaleFactor
        height: (rectHeight + 2 * leaderHeight + edgeBuffer) * displayScaleFactor
        visible: false
        z: 100
        clip: true
        color: "transparent"

        Canvas {
            id: canvas
            anchors.fill: parent

            property bool createPathAndPaint: false

            antialiasing: true            

            // work around for Qt bug with Canvas on iOS.
            // Rendering to Frame buffer object causes weirdness with size.
            Component.onCompleted: {
                if (Qt.platform.os === "ios")
                    renderTarget = Canvas.Image;
            }

            renderTarget: Canvas.FramebufferObject
            renderStrategy: Canvas.Cooperative

            // handler to override for drawing
            onPaint: {                
                screenCoordinates.x = borderWidth;
                screenCoordinates.y = borderWidth;
                drawCalloutFrame();
            }

            Rectangle {
                id: calloutContentFrame
                anchors {
                    left: parent.left
                    top: parent.top
                }

                GridLayout {
                    id: calloutLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                    }
                    height: 45 * displayScaleFactor
                    columns: 3
                    rows: 2
                    columnSpacing: 7 * displayScaleFactor

                    Rectangle {
                        id: imageRect
                        width: 40 * displayScaleFactor
                        height: width
                        color: "transparent"
                        Layout.rowSpan: 2
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                        Image {
                            id: image
                            source: calloutData ? calloutData.imageUrl : ""
                            width: parent.width
                            height: width
                            fillMode : Image.PreserveAspectFit
                        }
                    }

                    Text {
                        id: title
                        text: calloutData ? calloutData.title : ""
                        wrapMode: Text.NoWrap
                        renderType: Text.NativeRendering
                        color: titleTextColor
                        font {
                            pixelSize: 11 * displayScaleFactor
                            family: "sanserif"
                        }
                        clip: true
                        elide: Text.ElideRight
                        Layout.alignment: Qt.AlignVCenter
                        Layout.maximumWidth: !autoAdjustWidth ? titleWidth : Math.max(0, adjustedMaxWidth - 90 * displayScaleFactor) // resets to implicit width if non-autoAdjust
                    }

                    Rectangle {
                        id: accessoryButton
                        width: 40 * displayScaleFactor
                        height: width
                        color: "transparent"
                        Layout.rowSpan: 2

                        Image {
                            id: accessoryButtonImage
                            anchors.fill: parent
                            width: 40 * displayScaleFactor
                            height: width
                            fillMode: Image.PreserveAspectFit
                            visible: !accessoryButtonHidden
                        }

                        MouseArea {
                          id: region
                          anchors.fill: parent
                          onClicked: accessoryButtonClicked()
                          visible: !accessoryButtonHidden
                        }
                    }

                    Text {
                        id: detail
                        text: calloutData ? calloutData.detail : ""
                        renderType: Text.NativeRendering
                        color: detailTextColor
                        font {
                            pixelSize: 10 * displayScaleFactor
                            family: "sanserif"
                        }
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        Layout.alignment: Qt.AlignVCenter
                        Layout.maximumWidth: !autoAdjustWidth ? detailWidth : Math.max(0, adjustedMaxWidth - 90 * displayScaleFactor) // resets to implicit width if non-autoAdjust
                    }
                }
            }
        }
    }

    /*! \internal */
    // Draw the rounded rectangle with leader
    function drawCalloutFrame() {

        // Adjust the relative coordinates
        if (adjustedLeaderPosition === Enums.LeaderPosition.Top) {
            screenCoordinates.y += leaderHeight;
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Left) {
            screenCoordinates.x += leaderHeight;
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
            screenCoordinates.y += leaderHeight;
            screenCoordinates.x += cornerOffset;
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.LowerRight) {
            screenCoordinates.x += cornerOffset;
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            screenCoordinates.y += leaderHeight;
        }

        // get context to draw with
        var ctx = canvas.getContext("2d");

        // setup the style
        ctx.lineWidth = borderWidth;
        ctx.strokeStyle = borderColor;
        ctx.fillStyle = backgroundColor;
        ctx.alpha = 0.9;

        ctx.save();
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.lineJoin = "round";

        ctx.beginPath();

        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {            
            ctx.moveTo(screenCoordinates.x + leaderWidth + cornerOffset, screenCoordinates.y);
        } else {
            ctx.moveTo(screenCoordinates.x + internalCornerRadius, screenCoordinates.y);
        }

        // top side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Top) {
            ctx.lineTo(screenCoordinates.x + (halfRectWidth - halfLeaderWidth), screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + halfRectWidth, screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + (halfRectWidth + halfLeaderWidth), screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + rectWidth - internalCornerRadius, screenCoordinates.y);
        }

        // draw top right corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
            ctx.lineTo(screenCoordinates.x + rectWidth - leaderWidth - cornerOffset, screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + (rectWidth - halfLeaderWidth - cornerOffset), screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth - cornerOffset, screenCoordinates.y);
        }
        ctx.lineTo(screenCoordinates.x + rectWidth - internalCornerRadius, screenCoordinates.y);
        ctx.arcTo(screenCoordinates.x + rectWidth, screenCoordinates.y, screenCoordinates.x + rectWidth, screenCoordinates.y + internalCornerRadius, internalCornerRadius);

        // right side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Right) {
            ctx.lineTo(screenCoordinates.x + rectWidth, screenCoordinates.y + halfRectHeight - halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x + rectWidth + leaderHeight, screenCoordinates.y + halfRectHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth, screenCoordinates.y + halfRectHeight + halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x + rectWidth, screenCoordinates.y + rectHeight - internalCornerRadius);
        }
        ctx.arcTo(screenCoordinates.x + rectWidth, screenCoordinates.y + rectHeight, screenCoordinates.x + rectWidth - internalCornerRadius, screenCoordinates.y + rectHeight, internalCornerRadius);

        // draw bottom right corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.LowerRight) {
            ctx.lineTo(screenCoordinates.x + rectWidth - internalCornerRadius, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth - cornerOffset, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth - halfLeaderWidth - cornerOffset, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x + rectWidth - leaderWidth - cornerOffset, screenCoordinates.y + rectHeight);
        }

        // bottom side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Bottom) {
            ctx.lineTo(screenCoordinates.x + halfRectWidth+halfLeaderWidth, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + halfRectWidth, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x + halfRectWidth-halfLeaderWidth, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + internalCornerRadius, screenCoordinates.y + rectHeight);
        }

        // draw bottom left corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.LowerLeft) {
            ctx.lineTo(screenCoordinates.x + leaderWidth + cornerOffset, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + leaderWidth + cornerOffset, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + halfLeaderWidth + cornerOffset, screenCoordinates.y + rectHeight + leaderHeight);
            ctx.lineTo(screenCoordinates.x + cornerOffset, screenCoordinates.y + rectHeight);
            ctx.lineTo(screenCoordinates.x + internalCornerRadius, screenCoordinates.y + rectHeight);
        } else {
            ctx.lineTo(screenCoordinates.x + internalCornerRadius, screenCoordinates.y + rectHeight);
        }
        ctx.arcTo(screenCoordinates.x, screenCoordinates.y + rectHeight, screenCoordinates.x, screenCoordinates.y + rectHeight - internalCornerRadius, internalCornerRadius);

        // left side
        if (adjustedLeaderPosition === Enums.LeaderPosition.Left) {
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + halfRectHeight + halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x - leaderHeight, screenCoordinates.y + halfRectHeight);
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + halfRectHeight - halfLeaderWidth);
            ctx.lineTo(screenCoordinates.x, screenCoordinates.y + internalCornerRadius);
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            ctx.arcTo(screenCoordinates.x, screenCoordinates.y, screenCoordinates.x + internalCornerRadius + cornerOffset, screenCoordinates.y, internalCornerRadius);
        }

        // draw top left corner
        if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            ctx.lineTo(screenCoordinates.x + cornerOffset, screenCoordinates.y);
            ctx.lineTo(screenCoordinates.x + halfLeaderWidth + cornerOffset, screenCoordinates.y - leaderHeight);
            ctx.lineTo(screenCoordinates.x + leaderWidth + cornerOffset, screenCoordinates.y);
        } else {
            ctx.arcTo(screenCoordinates.x, screenCoordinates.y, screenCoordinates.x + internalCornerRadius, screenCoordinates.y, internalCornerRadius);
        }
        ctx.closePath();

        if (canvas.createPathAndPaint) {
            ctx.fill();
            ctx.stroke();
        }

        ctx.restore();
    }

    /*! \internal */
    // Changes the actual leader position (stored in adjustedLeaderPosition) if the current style specifies
    // Enums.LeaderPosition.Automatic and the current actual leader position isn't optimal for the current size and position of
    // the callout.
    function findBestLeaderPosition(mousex, mousey) {

        var refresh = false;
        var mapViewInsets = root.parent.viewInsets;

        if (debug) {
            console.log("mousex = ", mousex);
            console.log("mousey = ", mousey);
            console.log("calloutFrame.width = ", calloutFrame.width);
            console.log("calloutFrame.height = ", calloutFrame.height);
            console.log("root.parent.width = ", root.parent.width);
            console.log("root.parent.height = ", root.parent.height);
        }

        if (leaderPosition === Enums.LeaderPosition.Automatic) {

            // Move leader vertically if vertical position isn't optimal
            if (calloutFrame.y - edgeBuffer < 0 + mapViewInsets.top * displayScaleFactor) {
                // Top edge of callout is above top edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Up, mousex, mousey);
            }

            // Move leader horizontally if horizontal position isn't optimal
            if (calloutFrame.x - edgeBuffer < 0 + mapViewInsets.left * displayScaleFactor) {
                // Left edge of callout is left of left edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Left, mousex, mousey);
            } else if (calloutFrame.x + rectWidth + edgeBuffer > root.parent.width - mapViewInsets.right * displayScaleFactor) {
                // Right edge of callout is right of right edge of map
                refresh = moveLeader(Enums.LeaderMoveDirection.Right, mousex, mousey);
            }

        }

        return refresh;
    }

    /*! \internal */
    // Moves the leader position following one of 4 directions: Down, Up, Left or Right.
    // Return true if a new leaderPosition has been set.
    function moveLeader(direction, mousex, mousey) {
        var refresh = false;
        var mapViewInsets = root.parent.viewInsets;
        var mapViewWidth = root.parent.width;
        var mapViewHeight = root.parent.height;
        var anchorX = mousex;
        var anchorY = mousey;

        // Callout is 'narrow' if it's less than half the width of the MapView
        var narrowCallout = true;
        if (calloutFrame.width > (mapViewWidth + (mapViewInsets.left + mapViewInsets.right) * displayScaleFactor) / 2) {
          narrowCallout = false;
        }

        // Callout is 'short' if it's less than half the height of the MapView
        var shortCallout = true;
        if (calloutFrame.height > (mapViewHeight + (mapViewInsets.top + mapViewInsets.bottom) * displayScaleFactor) / 2) {
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
                if (narrowCallout || anchorX < mapViewWidth * 2 / 3) {
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

    /*! \internal */
    function setupAccessoryButton() {
        if (accessoryButtonType === "Info")
            accessoryButtonImage.source = "images/info-encircled.png";
        else if (accessoryButtonType === "Add")
            accessoryButtonImage.source = "images/add-encircled.png";
        else if (accessoryButtonType === "Custom")
            accessoryButtonImage.source = customImageUrl;
    }

    /*! \internal */
    function preCalculateWidthAndHeight() {

        // Calculate width and height of the rectangle with curved corners that we're going to draw.
        // This is the basic shape of the callout, but excluding the leader and not yet including extra room to take account
        // of the border having width.

        var calcMaxWidth = calloutContentMaxWidth() + 2 * internalCornerRadius;
        var calcMaxHeight = calloutContentMaxHeight() + 2 * internalCornerRadius;
        var minWidth = Math.min(calloutMinWidth, calcMaxWidth); // don't allow minWidth to be > maxWidth
        var minHeight = Math.min(calloutMinHeight, calcMaxHeight); // don't allow minHeight to be > maxHeight

        if (autoAdjustWidth) {
            // If we know the width of the content, base the width on that
            if (calloutLayout.width === 0) {
                rectWidth = minWidth;
            } else {
                rectWidth = calloutLayout.width + calloutFramePadding;
            }

            // If we know the height of the content, base the height on that
            rectHeight = calloutLayout.height;


            adjustedMaxWidth = maxWidth - (2 * leaderWidth) - edgeBuffer;
            adjustedMaxWidth = Math.max(calloutMinWidth, adjustedMaxWidth);

            if (rectWidth > maxWidth) {
                rectWidth = adjustedMaxWidth;
            }

        } else {
            // no auto adjust
            var adjustedCalloutWidth = calloutWidth - (2 * leaderWidth + edgeBuffer);
            rectWidth = Math.max(calloutMinWidth, adjustedCalloutWidth);
            rectHeight = calloutMinHeight;
        }

        if (debug) {
            console.log("rectWidth = ", rectWidth);
            console.log("rectHeight = ", rectHeight);
            console.log("minWidth = ", minWidth);
            console.log("minHeight = ", minHeight);
            console.log("calloutLayout.width = ", calloutLayout.width);
            console.log("calloutLayout.height = ", calloutLayout.height);
            console.log("calloutContentFrame.width = ", calloutContentFrame.width);
            console.log("dpi", Screen.pixelDensity * 25.4);
            console.log("detailWidth = ", detailWidth)
            console.log("titleWidth = ", titleWidth);
            console.log("imageWidth = ", imageWidth);
            console.log("maxWidth = ", maxWidth);
            console.log("calloutWidth = ", calloutWidth);
            console.log("adjustedMaxWidth = ", adjustedMaxWidth);
            console.log("displayScaleFactor = ", displayScaleFactor);
        }
    }

    /*! \internal */
    function adjustRelativePositionOfCanvasFrame(screenx, screeny, calloutWidth, calloutHeight) {

        if (adjustedLeaderPosition === Enums.LeaderPosition.Top ) {
            calloutContentFrame.anchors.leftMargin = 0;
            calloutContentFrame.anchors.topMargin = leaderHeight;
            calloutFrame.x = (screenx - calloutWidth / 2) - borderWidth;
            calloutFrame.y = screeny - borderWidth;
            if (debug) {
                console.log("top calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Bottom) {
            calloutContentFrame.anchors.topMargin = 0;
            calloutContentFrame.anchors.leftMargin = 0;
            calloutFrame.x = (screenx - (calloutWidth / 2)) - borderWidth;
            calloutFrame.y = (screeny  - (leaderHeight + calloutHeight)) - borderWidth;
            if (debug) {
                console.log("bottom calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Left) {
            calloutContentFrame.anchors.leftMargin = leaderHeight;
            calloutContentFrame.anchors.topMargin = 0;
            calloutFrame.x = screenx - borderWidth;
            calloutFrame.y = (screeny  - calloutHeight / 2) - borderWidth;
            if (debug) {
                console.log("left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.Right) {
            calloutContentFrame.anchors.leftMargin = 0;
            calloutContentFrame.anchors.topMargin = 0;
            calloutFrame.x = (screenx - (calloutWidth + leaderHeight)) - borderWidth;
            calloutFrame.y = (screeny  - calloutHeight / 2) - borderWidth;
            if (debug) {
                console.log("right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperRight) {
            calloutContentFrame.anchors.topMargin = leaderHeight;
            calloutContentFrame.anchors.leftMargin = leaderHeight;
            calloutFrame.x = (screenx - leaderWidth / 2 - (calloutWidth - leaderWidth)) - borderWidth;
            calloutFrame.y = screeny - borderWidth;
            if (debug) {
                console.log("upper right top right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.UpperLeft) {
            calloutContentFrame.anchors.leftMargin = 0;
            calloutContentFrame.anchors.topMargin = leaderHeight;
            calloutFrame.x = ((screenx - leaderWidth / 2) - cornerOffset) - borderWidth;
            calloutFrame.y = screeny - borderWidth;
            if (debug) {
                console.log("upper left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.LowerRight) {
            calloutContentFrame.anchors.topMargin = 0;
            calloutContentFrame.anchors.leftMargin = leaderHeight;
            calloutFrame.x = (screenx - calloutWidth + leaderWidth / 2) - borderWidth;
            calloutFrame.y = (screeny  - (leaderHeight + calloutHeight)) - borderWidth;
            if (debug) {
                console.log("lower right calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        } else if (adjustedLeaderPosition === Enums.LeaderPosition.LowerLeft) {
            calloutContentFrame.anchors.leftMargin = 0;
            calloutContentFrame.anchors.topMargin = 0;
            calloutFrame.x = ((screenx - leaderWidth / 2) - cornerOffset) - borderWidth;
            calloutFrame.y = (screeny  - (leaderHeight + calloutHeight)) - borderWidth;
            if (debug) {
                console.log("lower left calloutFrame = " , calloutFrame.x, " ", calloutFrame.y);
            }
        }

    }

    /*! \internal */
    function calloutContentMaxWidth() {
        // Start by getting width of MapView
        var maxWidthForMapView = root.parent.width;

        // Calculate space to allow for 2 corners and 1 border width.
        var widthOfExtras = borderWidth + (2 * internalCornerRadius);

        // Allow space for the leader, plus 2 extra leader lengths to ensure there's always room for some map to show around
        // the outside of the callout.
        widthOfExtras += (3 * leaderHeight);
        maxWidthForMapView -= widthOfExtras;

        // If max width has been specified and fits in MapView, just return that value
        var styleMaxWidthExclExtras = calloutWidth - widthOfExtras;
        if (styleMaxWidthExclExtras > 0 && styleMaxWidthExclExtras < maxWidthForMapView) {
            return styleMaxWidthExclExtras;
        }

        // Otherwise return max width that fits in MapView
        return maxWidthForMapView;
    }

    /*! \internal */
    function calloutContentMaxHeight() {
        // Start by getting height of MapView
        var maxHeightForMapView = root.parent.height;

        // Calculate space to allow for 2 corners and 1 border width.
        var heightOfExtras = borderWidth + (2 * internalCornerRadius);

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
