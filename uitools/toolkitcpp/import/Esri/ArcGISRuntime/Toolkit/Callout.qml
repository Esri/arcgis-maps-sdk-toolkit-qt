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

import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Shapes
import Esri.ArcGISRuntime.Toolkit.Controller

/*!
    \qmltype Callout
    \ingroup ArcGISQtToolkit
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
     \image callout.png
     \snippet qml/demos/CalloutDemoForm.qml Set up Callout QML

     \note That the Callout has gone through a major revision as of ArcGISRuntime 100.14.
     Part of this revision has been a change to the styling behaviour of the Callout, making the Callout
     compliant with your currently applied theme. To revert to the classic Callout look, you can supply the
     old style properties to the Callout as provided below.

     \code
            Callout {
              calloutData: myCalloutData
            }
     \endcode

     can be rewritten as:

     \code
            Callout {
              calloutData: myCalloutData
              palette.windowText: "#000000"
              background: Rectangle {
                  color: "#ffffff"
                  border.color: "#000000"
                  border.width: 2
                  radius: 5
              }
              leaderHeight: 10
              leaderWidth: 20
              leaderPosition: Callout.LeaderPosition.Bottom
            }
     \endcode
*/
Pane {
    id: root

    opacity: {
        if (!calloutData || calloutData.sceneLocationVisibility === undefined) {
            return 1.0;
        }
        const visibility = calloutData.sceneLocationVisibility;
        return (visibility === QmlEnums.SceneLocationVisibilityHiddenByBaseSurface ||
                visibility === QmlEnums.SceneLocationVisibilityHiddenByElevation) ? 0.5 : 1.0;
    }

    background: Rectangle {
        color: palette.base
        border.color: palette.dark
    }


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
        \brief The tooltip and accessible name of the accessory button.

        The default is the translated text "Show information" for "Info", "Add" for
        "Add", and an empty string for other button types. Set a meaningful action
        description when using a "Custom" button or overriding the default action.

        The tooltip is shown on hover or keyboard focus when this text is not empty.
    */
    property string accessoryButtonToolTip: accessoryButtonType === "Info" ? qsTr("Show information")
                                          : accessoryButtonType === "Add" ? qsTr("Add") : ""

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
        \brief The signal emitted when the accessory button is clicked.
    */
    signal accessoryButtonClicked()

    contentWidth: contentItem ? contentItem.implicitWidth : 0
    implicitHeight: Math.max(45, implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
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

    contentItem: Item {
        implicitWidth: root.autoAdjustWidth ? Math.min(internal.widthLimit, internal.naturalWidth) : internal.widthLimit
        implicitHeight: Math.max(textColumn.implicitHeight, internal.accessoryCount > 0 ? internal.accessorySize : 0)

        RowLayout {
            id: calloutLayout
            width: Math.min(parent.width, internal.widthLimit)
            height: parent.height
            spacing: 0

            Image {
                id: image
                source: root.calloutData ? root.calloutData.imageUrl : ""
                sourceSize: Qt.size(40, 40)
                fillMode: Image.PreserveAspectFit
                visible: internal.hasImage
                Layout.alignment: Qt.AlignVCenter
                Layout.minimumWidth: internal.accessorySize
                Layout.preferredWidth: internal.accessorySize
                Layout.maximumWidth: internal.accessorySize
                Layout.minimumHeight: internal.accessorySize
                Layout.preferredHeight: internal.accessorySize
                Layout.maximumHeight: internal.accessorySize
                Layout.rightMargin: internal.hasText || internal.hasButton ? internal.elementSpacing : 0
            }

            ColumnLayout {
                id: textColumn
                spacing: 5
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                Layout.preferredWidth: internal.textWidth
                Layout.maximumWidth: internal.textWidth
                Layout.rightMargin: internal.hasText && internal.hasButton ? internal.elementSpacing : 0

                Label {
                    id: title
                    text: root.calloutData ? root.calloutData.title : ""
                    textFormat: Text.PlainText
                    wrapMode: Text.NoWrap
                    maximumLineCount: 1
                    clip: true
                    elide: Text.ElideRight
                    visible: text.length > 0
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: internal.textWidth
                }

                Label {
                    id: detail
                    text: root.calloutData ? root.calloutData.detail : ""
                    textFormat: Text.PlainText
                    wrapMode: Text.NoWrap
                    maximumLineCount: 1
                    elide: Text.ElideRight
                    clip: true
                    visible: text.length > 0
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    Layout.maximumWidth: internal.textWidth
                }
            }

            RoundButton {
                id: accessoryButton
                Layout.alignment: Qt.AlignVCenter
                Layout.minimumWidth: internal.accessorySize
                Layout.preferredWidth: internal.accessorySize
                Layout.maximumWidth: internal.accessorySize
                Layout.minimumHeight: internal.accessorySize
                Layout.preferredHeight: internal.accessorySize
                Layout.maximumHeight: internal.accessorySize
                display: AbstractButton.IconOnly
                text: root.accessoryButtonToolTip
                hoverEnabled: true
                ToolTip.text: text
                ToolTip.visible: visible && enabled && text.length > 0 && (hovered || visualFocus)
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                icon.width: Math.min(24, internal.accessorySize)
                icon.height: Math.min(24, internal.accessorySize)
                flat: true
                radius: 32
                visible: internal.hasButton
                onClicked: root.accessoryButtonClicked()
                icon.source: internal.buttonImageSource
            }
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
                strokeColor: root.background.border.color
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
        readonly property bool hasImage: image.source.toString() !== ""
        readonly property url buttonImageSource: {
            if (root.accessoryButtonType === "Info")
                return "qrc:/Esri/ArcGISRuntime/Toolkit/information.svg";
            else if (root.accessoryButtonType === "Add")
                return "qrc:/Esri/ArcGISRuntime/Toolkit/plus-circle.svg";
            else if (root.accessoryButtonType === "Custom")
                return root.customImageUrl;

            return "";
        }
        readonly property bool hasButton: root.accessoryButtonVisible && buttonImageSource.toString() !== ""
        readonly property bool hasText: title.text.length > 0 || detail.text.length > 0
        readonly property int accessoryCount: (hasImage ? 1 : 0) + (hasButton ? 1 : 0)
        readonly property int gapCount: Math.max(0, accessoryCount + (hasText ? 1 : 0) - 1)
        readonly property real widthLimit: Math.max(0, root.maxWidth)
        readonly property real naturalWidth: accessoryCount * 40 + gapCount * 7
                                             + Math.ceil(Math.max(title.text.length > 0 ? title.implicitWidth : 0,
                                                                  detail.text.length > 0 ? detail.implicitWidth : 0))
        readonly property real elementSpacing: gapCount > 0
                                               ? Math.min(7, Math.max(0, calloutLayout.width - accessoryCount * 40) / gapCount) : 0
        readonly property real accessorySize: accessoryCount > 0
                                              ? Math.min(40, Math.max(0, calloutLayout.width - gapCount * elementSpacing) / accessoryCount) : 0
        readonly property real textWidth: Math.max(0, calloutLayout.width - accessoryCount * accessorySize - gapCount * elementSpacing)
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
            else if (anchorPointY < root.height + root.leaderHeight) {
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
    }
}
