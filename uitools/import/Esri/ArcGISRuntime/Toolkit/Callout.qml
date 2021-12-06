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
*/
Pane {
    id: root

    /*!
        \brief The enumerator for the set of possible positions of the leader line in the callout.

        LeaderPosition includes:

        \list
            \li LeaderPosition.UpperLeft (0)
            \li LeaderPosition.Top (1)
            \li LeaderPosition.UpperRight (2)
            \li LeaderPosition.Right (3)
            \li LeaderPositio.LowerRight (4)
            \li LeaderPosition.Bottom (5)
            \li LeaderPosition.LowerLeft (6)
            \li LeaderPosition.Left (7)
            \li LeaderPosition.Automatic (8)
        \endlist

        Automatic will decide the best placement, based on the
        location of the callout within the visible area of the MapView.

        The default is \c leaderPositionEnum.Bottom.
    */
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

    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief The property to set the leader position of the callout.

        For example, to set the leader line to the top of the callout use:

        \c Callout.leaderPosition: Callout.LeaderPosition.Top

        The default is \c Callout.LeaderPosition.Automatic.
    */
    property var leaderPosition: Callout.LeaderPosition.Automatic

    /*!
        \brief The height of the leader line in the Callout.

        The default leader height is \c 10.
    */
    property int leaderHeight: 10

    /*!
        \brief The width of the leader line in the Callout.

        The default leader width is \c 20.
    */
    property int leaderWidth: 20

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
        \brief A QML Item to display in the Callout.

        The default is \c null.
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
      When \c true, the width of the callout content automatically resizes up
      to the value of \l maxWidth. To explicitly set the width
      set autoAdjustWidth to \c false, and explicitly set \l implicitWidth.

      This property defaults to \c true.
    */
    property bool autoAdjustWidth: true

    /*!
      Maximum width of the Callout's conteent when `autoAdjustWidth` is true.

      This property defaults to \c 300.
    */
    property real maxWidth: 300

    /*!
        \obsolete
        Use \c{implicitWidth} instead.
    */
    property alias calloutWidth: root.implicitWidth

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
        tail.strokeColor = borderColor;
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
        Use \c{background.border.width} instead.
    */
    property int borderWidth
    onBorderWidthChanged: {
        background.border.width = borderWidth;
        tail.strokeWidth = borderWidth;
    }

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

    onImplicitWidthChanged: autoAdjustWidth = false
    implicitHeight: 100
    visible: false

    x: {
        switch(internal.leaderPosition) {
        case Callout.LeaderPosition.UpperLeft:
        case Callout.LeaderPosition.LowerLeft:
            return internal.anchorPointX - leaderWidth / 2 - padding;
        case Callout.LeaderPosition.UpperRight:
        case Callout.LeaderPosition.LowerRight:
            return internal.anchorPointX - root.width + leaderWidth / 2 + padding;
        case Callout.LeaderPosition.Top:
        case Callout.LeaderPosition.Bottom:
            return internal.anchorPointX - root.width / 2;
        case Callout.LeaderPosition.Left:
            return internal.anchorPointX + root.leaderHeight;
        case Callout.LeaderPosition.Right:
            return internal.anchorPointX - root.leaderHeight - root.width;
        }
    }

    y: {
        switch(internal.leaderPosition) {
        case Callout.LeaderPosition.UpperLeft:
        case Callout.LeaderPosition.Top:
        case Callout.LeaderPosition.UpperRight:
            return internal.anchorPointY + leaderHeight;
        case Callout.LeaderPosition.LowerRight:
        case Callout.LeaderPosition.Bottom:
        case Callout.LeaderPosition.LowerLeft:
            return internal.anchorPointY - height - leaderHeight;
        case Callout.LeaderPosition.Left:
        case Callout.LeaderPosition.Right:
            return internal.anchorPointY - height / 2;
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
            Layout.fillHeight: true
            Layout.preferredWidth: root.implictWidth ?? maxWidth
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
            visible: !calloutContent
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: !autoAdjustWidth
            Layout.fillHeight: true
            Layout.columnSpan: {
                let span = 1;
                if (!accessoryButton.visible)
                    span++;
                if (!image.visible)
                    span++;
                return span;
            }
            Layout.maximumWidth: maxWidth - (image.visible ? image.width : 0 ) - (accessoryButton.visible ? accessoryButton.width : 0)
        }
        RoundButton {
            id: accessoryButton
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 40
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
                else
                    return "";
            }
        }
        Label {
            id: detail
            text: calloutData ? calloutData.detail : ""
            wrapMode: Text.Wrap
            elide: Text.ElideRight
            visible: !calloutContent
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: !autoAdjustWidth
            Layout.fillHeight: true
            Layout.columnSpan: {
                let span = 1;
                if (!accessoryButton.visible)
                    span++;
                if (!image.visible)
                    span++;
                return span;
            }
            Layout.maximumWidth: maxWidth - (image.visible ? image.width : 0 ) - (accessoryButton.visible ? accessoryButton.width : 0)
        }
    }

    Shape {
        x: -leftPadding
        y: -topPadding
        z: 1
        ShapePath {
            // Hides the border of the Pane.
            strokeColor: tail.fillColor
            strokeWidth: tail.strokeWidth
            startX: {
                switch(internal.leaderPosition) {
                case Callout.LeaderPosition.UpperLeft:
                case Callout.LeaderPosition.Top:
                case Callout.LeaderPosition.UpperRight:
                case Callout.LeaderPosition.LowerRight:
                case Callout.LeaderPosition.Bottom:
                case Callout.LeaderPosition.LowerLeft:
                    return tail.startX + background.border.width;
                case Callout.LeaderPosition.Left:
                    return tail.startX + background.border.width / 2;
                case Callout.LeaderPosition.Right:
                    return tail.startX - background.border.width / 2;
                }
            }
            startY: {
                switch(internal.leaderPosition) {
                case Callout.LeaderPosition.UpperLeft:
                case Callout.LeaderPosition.Top:
                case Callout.LeaderPosition.UpperRight:
                    return tail.startY + background.border.width / 2;
                case Callout.LeaderPosition.LowerRight:
                case Callout.LeaderPosition.Bottom:
                case Callout.LeaderPosition.LowerLeft:
                    return tail.startY - background.border.width / 2;
                case Callout.LeaderPosition.Left:
                case Callout.LeaderPosition.Right:
                    return tail.startY + background.border.width;
                }
            }
            PathLine {
                relativeX: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return root.leaderWidth - background.border.width*2;
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return 0;
                    }
                }
                relativeY: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return 0;
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return root.leaderWidth - background.border.width*2;
                    }
                }
            }
        }
        ShapePath {
            // Draws the tail portion emitting from the pane.
            id: tail
            fillColor: root.background.color
            strokeColor: "transparent"
            strokeWidth: background.border.width
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
                    return root.width / 2 - root.leaderWidth / 2;
                }
            }
            startY: {
                switch(internal.leaderPosition) {
                case Callout.LeaderPosition.UpperLeft:
                case Callout.LeaderPosition.Top:
                case Callout.LeaderPosition.UpperRight:
                    return background.border.width/2;
                case Callout.LeaderPosition.LowerRight:
                case Callout.LeaderPosition.Bottom:
                case Callout.LeaderPosition.LowerLeft:
                    return root.height - background.border.width/2;
                case Callout.LeaderPosition.Left:
                case Callout.LeaderPosition.Right:
                    return root.height / 2 - root.leaderWidth / 2;
                }
            }
            PathLine {
                relativeX: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return root.leaderWidth / 2;
                    case Callout.LeaderPosition.Left:
                        return -root.leaderHeight;
                    case Callout.LeaderPosition.Right:
                        return root.leaderHeight;
                    }
                }
                relativeY: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                        return -root.leaderHeight;
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return root.leaderHeight;
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return root.leaderWidth / 2;
                    }
                }
            }
            PathLine {
                relativeX: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return root.leaderWidth / 2;
                    case Callout.LeaderPosition.Left:
                        return root.leaderHeight;
                    case Callout.LeaderPosition.Right:
                        return -root.leaderHeight;
                    }
                }
                relativeY: {
                    switch(internal.leaderPosition) {
                    case Callout.LeaderPosition.UpperLeft:
                    case Callout.LeaderPosition.Top:
                    case Callout.LeaderPosition.UpperRight:
                        return root.leaderHeight;
                    case Callout.LeaderPosition.LowerRight:
                    case Callout.LeaderPosition.Bottom:
                    case Callout.LeaderPosition.LowerLeft:
                        return -root.leaderHeight;
                    case Callout.LeaderPosition.Left:
                    case Callout.LeaderPosition.Right:
                        return root.leaderWidth / 2;
                    }
                }
            }
        }
    }

    QtObject {
        id: internal
        property real anchorPointX: (calloutData ? calloutData.screenPoint.x : 0) + screenOffsetX
        property real anchorPointY: (calloutData ? calloutData.screenPoint.y : 0) + screenOffsetY
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
    }
}
