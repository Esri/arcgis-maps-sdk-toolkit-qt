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

import QtQuick 2.12
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.15

/*!
    \qmltype Callout
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \ingroup ArcGISQtToolkitUiQmlViews
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \since Esri.ArcGISRuntime 100.10
    \brief A view for displaying information at a geographic location on a Map.

     A Callout can be displayed for several different scenarios:

     \list
        \li To display the coordinates where a user tapped on the map.
        \li To display information about a GeoElement that has been identified
        on the MapView.
        \li To display a callout at your current location.
     \endlist

     For more information, please see the CalloutData documentation.
     \image docs/callout.png
     Example code in the QML API (C++ API might differ):
     \snippet qml_quick/src/demos/CalloutDemoForm.qml Set up Callout QML

     \note That the Callout has gone through a major revision as of ArcGISRuntime 100.14.
     Part of this revision has been a change to the styling behaviour of the Callout, making the Callout
     compliant with your currently applied theme. To revert to the classic Callout look, you can supply the
     old style properties to the Callout as provided below.
     \oldcode
            Callout {
              calloutData: myCalloutData
            }
     \newcode
            Callout {
              calloutData: myCalloutData
              titleTextColor: "#000000"
              backgroundColor: "#ffffff"
              borderColor: "#000000"
              borderWidth: 2
              cornerRadius: 5
              leaderHeight: 10
              leaderWidth: 20
              leaderPosition: Callout.LeaderPosition.Bottom
            }
     \endcode
*/
Pane {
    id: root

    enum LeaderPosition {
        UpperLeft = 0,
        Top = 1,
        UpperRight = 2,
        Right = 3,
        LowerRight = 4,
        Bottom = 5,
        LowerLeft = 6,
        Left = 7,
        Automatic = 8
    }

    /*!
        \qmlproperty enumeration leaderPosition
        \brief The property to set the leader position of the callout.

        leaderPosition can be one of:
        \value Callout.LeaderPosition.UpperLeft \c{(0)}
        \value Callout.LeaderPosition.Top \c{(1)}
        \value Callout.LeaderPosition.UpperRight \c{(2)}
        \value Callout.LeaderPosition.Right \c{(3)}
        \value Callout.LeaderPosition.LowerRight \c{(4)}
        \value Callout.LeaderPosition.Bottom \c{(5)}
        \value Callout.LeaderPosition.LowerLeft \c{(6)}
        \value Callout.LeaderPosition.Left \c{(7)}
        \value Callout.LeaderPosition.Automatic \c{(8)} The default.

        \c Callout.LeaderPosition.Automatic will decide the best placement,
        based on the location of the callout within the visible area of the MapView.

        The default is \c Callout.LeaderPosition.Automatic.
    */
    property var leaderPosition: Callout.LeaderPosition.Automatic

    /*!
        \brief The height of the leader line in the Callout.

        The default leader height is \c 15.
    */
    property int leaderHeight: 15

    /*!
        \brief The width of the leader line in the Callout.

        The default leader width is \c 30.
    */
    property int leaderWidth: 30

    /*!
        \brief The x offset of the placement of the Callout.

        The default is \c 0.
    */
    property int screenOffsetX: 0

    /*!
        \brief The y offset of the placement of the Callout.

        The default is \c 0.
    */
    property int screenOffsetY: 0

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
    property string customImageUrl: ""

    /*!
        \brief Whether to show the accessoryButton of the Callout.

        The default is \c true.
    */
    property bool accessoryButtonVisible: true

    /*!
        \obsolete

        \brief A QML Item to display in the Callout.

        The default is \c null.

        This property is obsolete, to replace the Callout's content
        set \c{contentItem} instead.
    */
    property Component calloutContent: null

    /*!
        \brief The CalloutData to display in the Callout.

        The CalloutData controls the data that is being displayed
        in the Callout view. Use CalloutData to set title text,
        detail text, images, GeoElements, and so on.
        CalloutData is obtained from the MapView.
    */
    property var calloutData: null
    onCalloutDataChanged: {
        if (calloutData === null) {
            dismiss();
        } else if (calloutData.visible) {
            showCallout();
        }
    }

    /*!
      \brief When \c true, the width of the callout content automatically resizes up
      to the value of \l maxWidth. When \c false, the content width will fixed
      to the size of \l maxWidth.

      This property defaults to \c true.
    */
    property bool autoAdjustWidth: true

    /*!
      \brief The width of the callout contents.

      When \l autoAdjustWidth is \c false, the width of the
      callout content will be fixed to this value.

      When \l autoAdjustWidth is \c true, the content width is calculated dynamically
      and may be smaller than this value, but will be no greater than this value.

      This property defaults to \c 300.
    */
    property real maxWidth: 300

    /*!
        \obsolete
        Use \c{implicitHeight} instead.
    */
    property alias calloutHeight: root.implicitHeight

    /*!
        \obsolete
        Use \c{background.border.color} instead.
    */
    property color borderColor
    onBorderColorChanged: {
        background.border.color = borderColor;
        internal.leaderColor = borderColor;
    }

    /*!
        \obsolete
        Use \c{Callout.LeaderPosition} instead.
    */
    property var leaderPositionEnum: { return {
            UpperLeft: 0,
            Top: 1,
            UpperRight: 2,
            Right: 3,
            LowerRight: 4,
            Bottom: 5,
            LowerLeft: 6,
            Left: 7,
            Automatic: 8
        } }

    /*!
        \obsolete
        Use \l maxWidth instead.
    */
    property alias calloutWidth: root.maxWidth

    /*!
        \obsolete
        Use \c{background.border.width} instead.
    */
    property int borderWidth
    onBorderWidthChanged: background.border.width = borderWidth;

    /*!
        \obsolete
        Use \c{background.color} instead.
    */
    property color backgroundColor
    onBackgroundColorChanged: background.color = backgroundColor

    /*!
        \obsolete
        Use \c{palette.windowText} instead.
    */
    property color titleTextColor
    onTitleTextColorChanged: palette.windowText = titleTextColor

    /*!
        \obsolete
        Use \c{palette.windowText} instead.
    */
    property color detailTextColor
    onDetailTextColorChanged: palette.windowText = detailTextColor

    /*!
        \obsolete
        Use \c{accessoryButtonVisible} instead.
    */
    property bool accessoryButtonHidden: false
    onAccessoryButtonHiddenChanged: accessoryButtonVisible = !accessoryButtonHidden

    /*!
        \obsolete
        Use \c{background.radius} instead.
    */
    property int cornerRadius
    onCornerRadiusChanged: background.radius = cornerRadius

    /*!
        \brief The signal emitted when the accessory button is clicked.
    */
    signal accessoryButtonClicked()

    implicitHeight: 100
    visible: false

    x: {
        switch(internal.leaderPosition) {
        case Callout.LeaderPosition.Left:
            return internal.anchorPointX + root.leaderHeight;
        case Callout.LeaderPosition.Right:
            return internal.anchorPointX - root.leaderHeight - root.width;
        case Callout.LeaderPosition.UpperLeft:
        case Callout.LeaderPosition.LowerLeft:
            return internal.anchorPointX - leaderWidth / 2 - padding;
        case Callout.LeaderPosition.UpperRight:
        case Callout.LeaderPosition.LowerRight:
            return internal.anchorPointX - root.width + leaderWidth / 2 + padding;
        case Callout.LeaderPosition.Top:
        case Callout.LeaderPosition.Bottom:
        default:
            return internal.anchorPointX - root.width / 2;
        }
    }

    y: {
        switch(internal.leaderPosition) {
        case Callout.LeaderPosition.Left:
        case Callout.LeaderPosition.Right:
            return internal.anchorPointY - height / 2;
        case Callout.LeaderPosition.UpperLeft:
        case Callout.LeaderPosition.Top:
        case Callout.LeaderPosition.UpperRight:
            return internal.anchorPointY + leaderHeight;
        case Callout.LeaderPosition.LowerRight:
        case Callout.LeaderPosition.Bottom:
        case Callout.LeaderPosition.LowerLeft:
        default:
            return internal.anchorPointY - height - leaderHeight;
        }
    }

    /*!
        \brief Show the Callout on the MapView.

        Before showing the callout, set your desired properties for
        CalloutData (which controls the information that is displayed)
        and for Callout (which controls how the view appears on the MapView).
    */
    function showCallout() {
        // no calloutData set
        if (calloutData)
            root.visible = true;
    }

    /*!
        \brief Dismisses the Callout from the MapView.

        The Callout does not hide itself automatically, so you must
        explicitly call this method to hide the Callout from the
        MapView.
    */
    function dismiss() {
        root.visible = false;
    }

    Component.onCompleted: {
        background.children.push(shapeTail.createObject())
    }

    contentItem: GridLayout {
        id: calloutLayout
        columns: 3
        rows: 2
        columnSpacing: 7
        Loader {
            sourceComponent: calloutContent
            clip: true
            Layout.columnSpan: 3
            Layout.rowSpan: 2
            Layout.fillWidth: true
            Layout.preferredWidth: autoAdjustWidth ? -1 : root.maxWidth
            Layout.maximumWidth: autoAdjustWidth ? root.maxWidth : -1
            visible: calloutContent
        }
        Image {
            id: image
            source: calloutData ? calloutData.imageUrl : ""
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.preferredWidth: 40
            fillMode : Image.PreserveAspectFit
            visible: !calloutContent && source && source.toString() !== ""
        }
        Label {
            id: title
            text: calloutData ? calloutData.title : ""
            wrapMode: Text.Wrap
            clip: true
            elide: Text.ElideRight
            // Is visible (even when empty) if detail is visible, otherise
            // row & columnspan offsets go askew.
            visible: !calloutContent && (text || detail.visible)
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: autoAdjustWidth ? -1 : internal.labelWidthFrom.bind(this)(root.maxWidth)
            Layout.maximumWidth: autoAdjustWidth ? internal.labelWidthFrom.bind(this)(root.maxWidth) : -1
            Layout.columnSpan: {
                let span = 1;
                if (!accessoryButton.visible)
                    span++;
                if (!image.visible)
                    span++;

                return span;
            }
            Layout.rowSpan: {
                let span = 1;
                if (!detail.visible)
                    span++;

                return span;
            }
        }
        RoundButton {
            id: accessoryButton
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 40
            Layout.columnSpan: {
                let span = 1;
                if (!title.visible && detail.visible)
                    span++;

                return span;
            }
            display: AbstractButton.IconOnly
            topPadding: 0
            bottomPadding: 0
            leftPadding: 0
            rightPadding: 0
            flat: true
            radius: 32
            visible: accessoryButtonVisible && !calloutContent && icon.source.toString() !== ""
            onClicked: accessoryButtonClicked()
            icon.source: {
                if (accessoryButtonType === "Info")
                    return "images/information.svg";
                else if (accessoryButtonType === "Add")
                    return "images/plus-circle.svg";
                else if (accessoryButtonType === "Custom")
                    return customImageUrl;

                return "";
            }
        }
        Label {
            id: detail
            text: calloutData ? calloutData.detail : ""
            wrapMode: Text.Wrap
            elide: Text.ElideRight
            clip: true
            visible: !calloutContent && text
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: autoAdjustWidth ? -1 : internal.labelWidthFrom.bind(this)(root.maxWidth)
            Layout.maximumWidth: autoAdjustWidth ? internal.labelWidthFrom.bind(this)(root.maxWidth) : -1
        }
    }

    Component {
        id: shapeTail
        Shape {
            z: 1
            ShapePath {
                id: hideLine
                // Hides the border of the Pane.
                strokeColor: tail.fillColor
                // Only draw the line when a leaderWidth is available.
                strokeWidth: root.leaderWidth <= 0 ? -1 : (tail.strokeWidth)
                capStyle: ShapePath.RoundCap
                readonly property real leaderAngle: Math.atan(leaderHeight * 2 / leaderWidth)
                readonly property real leaderXOffset: Math.cos(leaderAngle) * (tail.strokeWidth / 2)
                startX: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.Left:
                        return tail.startX + background.border.width / 2;
                    case Callout.LeaderPosition.Right:
                        return tail.startX - background.border.width / 2;
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.LowerLeft:
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.Bottom:
                    default:
                        return tail.startX + leaderXOffset;
                    }
                }
                startY: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return tail.startY + leaderXOffset;
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                        return tail.startY + background.border.width / 2;
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                    default:
                        return tail.startY - background.border.width / 2
                    }
                }
                PathLine {
                    relativeX: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                        case Callout.LeaderPosition.Right:
                            return 0;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return root.leaderWidth - hideLine.leaderXOffset * 2;
                        }
                    }
                    relativeY: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                        case Callout.LeaderPosition.Right:
                            return root.leaderWidth - hideLine.leaderXOffset * 2;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return 0;
                        }
                    }
                }
            }
            ShapePath {
                // Draws the tail portion emitting from the pane.
                id: tail
                fillColor: root.background.color
                strokeColor: internal.leaderColor
                strokeWidth: parent.border.width
                capStyle: ShapePath.RoundCap
                startX: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.LowerLeft:
                        return padding;
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                        return root.width - root.leaderWidth - padding;
                    case Callout.LeaderPosition.Left:
                        return background.border.width/2;
                    case Callout.LeaderPosition.Right:
                        return root.width - background.border.width/2;
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.Bottom:
                    default:
                        return root.width / 2 - root.leaderWidth / 2;
                    }
                }
                startY: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return root.height / 2 - root.leaderWidth / 2;
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                        return background.border.width/2;
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                    default:
                        return root.height - background.border.width/2;
                    }
                }
                PathLine {
                    relativeX: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                            return -root.leaderHeight;
                        case Callout.LeaderPosition.Right:
                            return root.leaderHeight;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return root.leaderWidth / 2;
                        }
                    }
                    relativeY: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                        case Callout.LeaderPosition.Right:
                            return root.leaderWidth / 2;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                            return -root.leaderHeight;
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return root.leaderHeight;
                        }
                    }
                }
                PathLine {
                    relativeX: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                            return root.leaderHeight;
                        case Callout.LeaderPosition.Right:
                            return -root.leaderHeight;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return root.leaderWidth / 2;
                        }
                    }
                    relativeY: {
                        switch(internal.leaderPosition) {
                        case Callout.LeaderPosition.Left:
                        case Callout.LeaderPosition.Right:
                            return root.leaderWidth / 2;
                        case Callout.LeaderPosition.UpperLeft:
                        case Callout.LeaderPosition.Top:
                        case Callout.LeaderPosition.UpperRight:
                            return root.leaderHeight;
                        case Callout.LeaderPosition.LowerRight:
                        case Callout.LeaderPosition.Bottom:
                        case Callout.LeaderPosition.LowerLeft:
                        default:
                            return -root.leaderHeight;
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal
        property real anchorPointX: (calloutData ? calloutData.screenPoint.x : 0) + screenOffsetX
        property real anchorPointY: (calloutData ? calloutData.screenPoint.y : 0) + screenOffsetY
        // QML bug workaround here: `background.border.color` always comes out as black by default,
        // even if it is transparent. We keep the leader color as transparent until explicilty set via
        // `borderColor`. This supports old behaviour, and also supports all current known styles where
        // Pane does not have an initial background border color.
        property color leaderColor: "transparent"
        // Is either the contents of root.leaderPosition, or a calculated LeaderPosition if root.leaderPosition
        // is set to \c Automatic.
        property int leaderPosition: {
            if (root.leaderPosition !== Callout.LeaderPosition.Automatic) {
                return root.leaderPosition;
            } else if (anchorPointX < root.width / 2 && anchorPointY < (root.height + leaderHeight)) {
                return Callout.LeaderPosition.UpperLeft;
            }
            else if (anchorPointX > (root.parent.width - root.width / 2) && anchorPointY < (root.height + leaderHeight)) {
                return Callout.LeaderPosition.UpperRight;
            }
            else if (anchorPointX > (root.parent.width - root.width / 2) && anchorPointY > (root.parent.height - (root.height + leaderHeight))) {
                return Callout.LeaderPosition.LowerRight;
            }
            else if (anchorPointX < root.width / 2  && anchorPointY > (root.parent.height - (root.height + leaderHeight))) {
                return Callout.LeaderPosition.LowerLeft;
            }
            else if (anchorPointX > (root.parent.width - root.width / 2)) {
                return Callout.LeaderPosition.Right;
            }
            else if (anchorPointX < root.width / 2) {
                return Callout.LeaderPosition.Left;
            }
            else if (anchorPointY < root.height) {
                return Callout.LeaderPosition.Top;
            }

            return Callout.LeaderPosition.Bottom;
        }
        // Keeps track of when to show/hide callout based on CalloutData.
        property Connections calloutDataConnections: Connections {
            target: root.calloutData
            function onVisibleChanged() {
                if (root.calloutData.visible) {
                    root.showCallout();
                } else {
                    root.dismiss();
                }
            }
        }
        // Calculate the portion of property `width` allocated to the label `this`.
        // Called via `labelWidthFrom.bind(label)(width)`.
        function labelWidthFrom(width) {
            if (!this.text)
                return 0;

            return width - (image.visible ? image.width : 0 ) - (accessoryButton.visible ? accessoryButton.width : 0);
        }
    }
}
