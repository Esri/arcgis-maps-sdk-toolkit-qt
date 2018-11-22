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

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.2
import Esri.ArcGISRuntime.Toolkit.CppApi 100.3

/*!
    \qmltype CoordinateConversion
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \ingroup ToolCoordinateConversion
    \since Esri.ArcGISRutime 100.2
    \brief The user interface for the coordinate conversion tool.
    \sa {Coordinate Conversion Tool}
*/

Item {
    id: coordinateConversionWindow

    /*!
      \qmlproperty int textColor
      \brief The color of coordinate notation text and labels on this tool.

      The default value is \c "black".
     */
    property color textColor: "black"

    /*!
      \qmlproperty GeoView geoView
      \brief The GeoView for this tool. Should be a SceneQuickView or a MapQuickView.
     */
    property var geoView: null

    onGeoViewChanged: coordinateConvController.setGeoView(geoView);

    /*!
      \qmlproperty bool captureMode
      \brief Whether whether the tool is in capture mode.

      If \c true, the tool will convert a point set via a mouse click.
      If \c false, the too will use the app's current location as the target point.
     */
    property alias captureMode: coordinateConvController.captureMode

    /*!
      \qmlproperty real backgroundOpacity
      \brief The opacity of the background rectangle.
      */
    property alias backgroundOpacity: backgroundRectangle.opacity

    /*!
      \qmlproperty string inputFormat
      \brief The input format for the tool. This can be in a user defined format or one of:

      \list
        \li \c DD. Decimal degrees.
        \li \c DDM. Degrees decimal minutes.
        \li \c DMS. Degrees minutes seconds.
        \li \c MGRS.
        \li \c USNG.
        \li \c UTM.
        \li \c GARS
        \li GeoRef
      \endlist
     */
    property alias inputFormat: coordinateConvController.inputFormat

    /*!
      \qmlproperty int highlightColor
      \brief The color of used to highlight UI elements in this tool.

      The default value is \c "blue".
     */
    property color highlightColor: "blue"

    /*!
      \qmlproperty int backgroundColor
      \brief The color of used to for background UI elements in this tool.

      The default value is \c "blue".
     */
    property color backgroundColor: "lightgrey"

    /*!
      \qmlproperty int fontSize
      \brief The font size of coordinate notation text on this tool.

      The default value is \c 12.
     */
    property int fontSize: 12

    /*!
      \qmlproperty int fontFamily
      \brief The font family for text on this tool.

      The default is \c "helvetica".
     */
    property string fontFamily: "helvetica"

    /*!
      \qmlproperty bool expandUpwards
      \brief Whether the tool should expand upwards as new UI elements are added.

      The default value is \c true.
     */
    property bool expandUpwards: true

    /*!
      \qmlproperty real radius
      \brief This property holds the corner radius used to draw a rounded rectangle.

      The default value is \c 0.
     */
    property alias radius: backgroundRectangle.radius

    CoordinateConversionController {
        id: coordinateConvController
        objectName: "coordinateConversionController"
        active: coordinateConversionWindow.visible

        onActiveChanged: {
            if (!active && coordinateConversionWindow.visible)
                coordinateConversionWindow.visible = false;
            else if (active && !coordinateConversionWindow.visible)
                coordinateConversionWindow.visible = true;
        }
    }

    Rectangle {
        id: backgroundRectangle
        anchors {
            top: menuButton.checked ? expandUpwards ? addConversionButton.top : inputModeButton.top : inputModeButton.top
            bottom: menuButton.checked ? expandUpwards ? inputModeButton.bottom : results.bottom : inputModeButton.bottom
            left: parent.left
            right: parent.right
        }
        color: backgroundColor
    }

    TextMetrics {
        id: textMetrics
        font {
            bold: true
            family: fontFamily
            pixelSize: coordinateConversionWindow.fontSize
        }
        text: "MMMMMM"
    }

    Button {
        id: inputModeButton
        anchors {
            top: expandUpwards ? undefined : parent.top
            left: parent.left
            bottom: expandUpwards ? parent.bottom : undefined
            margins: 5
        }
        height: 32
        width: textMetrics.width
        text: coordinateConvController.inputFormat.length > 0 ? coordinateConvController.inputFormat : "Set format"
        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }

        contentItem: Text {
            text: inputModeButton.text
            font {
                bold: true
                family: fontFamily
                pixelSize: coordinateConversionWindow.fontSize
            }
            color: textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onClicked: {
            inputModesMenu.visible = true;
        }

        Menu {
            id: inputModesMenu
            x: inputModeButton.x
            y: expandUpwards ? -implicitHeight : inputModeButton.height
            visible: false

            Repeater {
                model: coordinateConvController.coordinateFormats

                delegate: Button {
                    id: inputModeOptionButton
                    text: modelData.toUpperCase()
                    anchors {
                        left: parent.left
                    }

                    background: Rectangle {
                        color: text === inputModeButton.text ? highlightColor : "transparent"
                    }

                    contentItem: Text {
                        text: inputModeOptionButton.text.toUpperCase()
                        font {
                            family: fontFamily
                            pixelSize: coordinateConversionWindow.fontSize
                        }
                        color: textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    onClicked: {
                        coordinateConvController.inputFormat = modelData;
                        inputModesMenu.close();
                    }
                }
            }
        }
    }

    Text {
        id: pointToConvertEntry
        visible: !editCoordinateButton.checked
        anchors {
            left: inputModeButton.right
            verticalCenter: inputModeButton.verticalCenter
            right: menuButton.left
        }
        height: inputModeButton.height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

        text: coordinateConvController.pointToConvert.length > 0 ? coordinateConvController.pointToConvert : "No position"
        font{
            family: fontFamily
            pixelSize: coordinateConversionWindow.fontSize
        }
        wrapMode: Text.Wrap
        elide: Text.ElideRight
        color: textColor
    }

    TextField {
        id: editPointEntry
        visible: editCoordinateButton.checked
        anchors {
            left: inputModeButton.right
            verticalCenter: inputModeButton.verticalCenter
            right: menuButton.left
            leftMargin: 5
        }

        placeholderText: "No position"
        text: coordinateConvController.pointToConvert
        font{
            family: fontFamily
            pixelSize: coordinateConversionWindow.fontSize
        }
        color: highlightColor

        onAccepted: {
            coordinateConvController.convertNotation(text);
            editCoordinateButton.checked = false;
        }
    }

    Button {
        id: menuButton

        anchors {
            verticalCenter: inputModeButton.verticalCenter
            right: parent.right
            margins: 5
        }
        height: inputModeButton.height
        width: height

        checkable: true
        checked: false
        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: menuButton
            source: menuButton.checked ? (expandUpwards ? "images/menuCollapse.png" : "images/menuExpand.png") :
                                         (expandUpwards ? "images/menuExpand.png" : "images/menuCollapse.png")
        }
    }

    Button {
        id: addConversionButton
        anchors {
            left: parent.left
            bottom: expandUpwards ? results.top : undefined
            top: expandUpwards ? undefined : inputModeButton.bottom
            right: zoomToButton.left
        }
        height: inputModeButton.height
        visible: menuButton.checked
        clip: true
        background: Rectangle {
            color: addConversionButton.down ? highlightColor : "transparent"
        }

        contentItem: Text {
            text: "Add conversion"
            font{
                family: fontFamily
                pixelSize: coordinateConversionWindow.fontSize
            }
            color: textColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            wrapMode: Text.WrapAnywhere
        }

        onClicked: {
            addConversionMenu.visible = true;
        }

        Menu {
            id: addConversionMenu
            x: addConversionButton.x
            y: expandUpwards ? -implicitHeight : addConversionButton.height
            visible: false

            Repeater {
                model: coordinateConvController.coordinateFormats

                delegate: Button {
                    id: addConversionOptionButton
                    text: modelData
                    enabled: text !== inputModeButton.text
                    opacity: enabled ? 1.0 : 0.5
                    anchors {
                        left: parent.left
                    }

                    contentItem: Text {
                        text: addConversionOptionButton.text.toUpperCase()
                        font{
                            family: fontFamily
                            pixelSize: coordinateConversionWindow.fontSize
                        }
                        color: textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    onClicked: {
                        coordinateConvController.addCoordinateFormat(modelData);
                        addConversionMenu.close();
                    }
                }
            }
        }
    }

    Button {
        id: zoomToButton

        anchors {
            verticalCenter: addConversionButton.verticalCenter
            right: flashCoordinateButton.left
        }
        height: addConversionButton.height
        width: height

        visible: menuButton.checked

        background: Rectangle {
            color: zoomToButton.down ? highlightColor : "transparent"
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: zoomToButton
            source: "images/Zoom.png"
        }

        onClicked: {
            coordinateConvController.zoomTo();
        }
    }

    Button {
        id: flashCoordinateButton

        anchors {
            verticalCenter: addConversionButton.verticalCenter
            right: editCoordinateButton.left
        }
        height: addConversionButton.height
        width: height

        visible: geoView && menuButton.checked

        background: Rectangle {
            color: flashCoordinateButton.down ? highlightColor : "transparent"
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: flashCoordinateButton
            source: "images/flash.png"
        }

        onClicked: {
            if (geoView === null)
                return;

            var screenPos = coordinateConvController.screenCoordinate();
            if (screenPos.x === -1.0 && screenPos.y === -1.0)
                return;

            var itemPos = geoView.mapToItem(coordinateConversionWindow, screenPos.x, screenPos.y);
            flashImage.x = itemPos.x - (flashImage.width * 0.5);
            flashImage.y = itemPos.y - (flashImage.height * 0.5);
            flashImage.running = true;
        }
    }

    Button {
        id: editCoordinateButton

        anchors {
            verticalCenter: addConversionButton.verticalCenter
            right: captureModeButton.left
        }
        height: addConversionButton.height
        width: height

        visible: menuButton.checked
        checkable: true

        background: Rectangle {
            color: "transparent"
            border {
                color: editCoordinateButton.checked ? highlightColor : "transparent"
                width: 1
            }
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            sourceSize.height: parent.width
            height: sourceSize.height
            opacity: editCoordinateButton.checked ? 1.0 : 0.5
            source: "images/Text_Editing_Mode.png"
        }
    }

    Button {
        id: captureModeButton

        anchors {
            verticalCenter: addConversionButton.verticalCenter
            right: parent.right
        }
        height: addConversionButton.height
        width: height

        visible: menuButton.checked
        checkable: true
        checked: coordinateConvController.captureMode

        background: Rectangle {
            color: "transparent"
            border {
                color: captureModeButton.checked ? highlightColor : "transparent"
                width: 1
            }
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            sourceSize.height: parent.width
            height: sourceSize.height
            opacity: captureModeButton.checked ? 1.0 : 0.5
            source: "images/Mouse_Click_Mode.png"
        }

        onCheckedChanged: {
            if (coordinateConvController.captureMode !== checked)
                coordinateConvController.captureMode = checked;
        }
    }

    ListView {
        id: results
        anchors {
            bottom: expandUpwards ? inputModeButton.top : undefined
            top: expandUpwards ? undefined: addConversionButton.bottom
            left: inputModeButton.left
            right: parent.right
        }

        visible: menuButton.checked
        height: count * inputModeButton.height
        model: coordinateConvController.results

        delegate:
            Item {
            height: inputModeButton.height
            width: results.width

            Text {
                id: formatName
                text: name
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                width: inputModeButton.width
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font {
                    family: fontFamily
                    pixelSize: coordinateConversionWindow.fontSize
                }
                color: textColor
                wrapMode: Text.Wrap
                elide: Text.ElideRight
            }

            Text {
                id: formatNotation
                text: notation
                anchors {
                    left: formatName.right
                    verticalCenter: parent.verticalCenter
                    right: editMenuButton.left
                }
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font{
                    family: fontFamily
                    pixelSize: coordinateConversionWindow.fontSize
                }
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                color: textColor
            }

            Button {
                id: editMenuButton
                width: height
                height: inputModeButton.height

                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 5
                }
                background: Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: editMenuButton
                    sourceSize.height: editMenuButton.width
                    height: sourceSize.height
                    source: "images/menu.png"
                }

                onClicked: {
                    editMenu.y = editMenuButton.y;
                    editMenu.currentName = name;
                    editMenu.currentNotation = notation;
                    editMenu.open();
                }
            }
        }

        Menu {
            id: editMenu
            visible: false
            x: results.width - width
            property string currentName: ""
            property string currentNotation: ""
            width: 100

            Column {
                width: parent.width
                anchors.margins: 10
                spacing: 10
                leftPadding: 10

                Label {
                    text: "Delete"
                    font {
                        family: fontFamily
                        pixelSize: coordinateConversionWindow.fontSize
                    }
                    color: textColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            editMenu.close();
                            coordinateConvController.removeCoordinateFormat(editMenu.currentName);
                        }
                    }
                }

                Label {
                    text: "Copy"
                    font {
                        family: fontFamily
                        pixelSize: coordinateConversionWindow.fontSize
                    }
                    color: textColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            editMenu.close();
                            coordinateConvController.copyToClipboard(editMenu.currentNotation);
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: flashImage
        property alias running: animation.running

        opacity: 0.0
        height: 16
        width: height
        radius: height
        color: highlightColor
        border {
            color: "transparent"
            width: 1
        }

        SequentialAnimation {
            id: animation
            running: false
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
        }
    }
}
