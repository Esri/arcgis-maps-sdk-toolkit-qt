import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.2
import Esri.ArcGISRuntime.Toolkit.CppApi 100.2

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
      \qmlproperty real scaleFactor
      \brief The scale factor used for sizing UI elements.

      Pixel density and screen resolution varies greatly between different
      devices and operating systems. This property allows your app to specify
      the width or height of UI elements so that the sizes appear similar
      (relative to screen size) across devices. Here is an example of how to
      use this property.

      \code
         CoordinateConversion {
            buttonWidth: 90 * scaleFactor
            spacingValue: 8 * scaleFactor
            ...
         }
      \endcode
     */
    property real scaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    /*!
      \qmlproperty int textColor
      \brief The color of coordinate notation text and labels on this tool.

      The default value is \c "black".
     */
    property color textColor: "black"

    property var geoView: null

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
    property color backgroundColor: "white"

    /*!
      \qmlproperty int fontSize
      \brief The font size of coordinate notation text on this tool.

      The default value is \c 12.
     */
    property int fontSize: 12

    /*!
      \qmlproperty int fontFamily
      \brief The font family for text on this tool.
     */
    property string fontFamily: ""

    /*!
      \qmlproperty bool expandUpwards
      \brief Whether the tool should expand upwards as new UI elements are added.

      The default value is \c true.
     */
    property bool expandUpwards: true

    property alias radius: backgroundRecatangle.radius

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
        id: backgroundRecatangle
        anchors{
            top: menuButton.checked ? expandUpwards ? addConversionButton.top : inputModeButton.top : inputModeButton.top
            bottom: menuButton.checked ? expandUpwards ? inputModeButton.bottom : addConversionButton.bottom : inputModeButton.bottom
            left: parent.left
            right: parent.right
        }
        color: backgroundColor
        opacity: coordinateConversionWindow.opacity
    }

    Button {
        id: inputModeButton
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        width: implicitWidth
        text: coordinateConvController.inputFormat.length > 0 ? coordinateConvController.inputFormat : "Set format"

        contentItem: Text {
            text: inputModeButton.text
            font{
                bold: true
                family: fontFamily
                pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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

                delegate: Button{
                    id: inputModeOptionButton
                    text: modelData.toUpperCase()
                    anchors{
                        left: parent.left
                    }

                    background: Rectangle {
                        color: text === inputModeButton.text ? highlightColor : "transparent"
                    }

                    contentItem: Text {
                        text: inputModeOptionButton.text.toUpperCase()
                        font{
                            family: fontFamily
                            pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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
            pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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
        }

        placeholderText: "No position"
        text: coordinateConvController.pointToConvert
        font{
            family: fontFamily
            pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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
        }
        height: inputModeButton.height
        width: height

        checkable: true
        checked: false

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: menuButton
            sourceSize.height: menuButton.width
            height: sourceSize.height
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
        }
        visible: menuButton.checked
        text: "Add conversion"

        background: Rectangle {
            color: addConversionButton.down ? highlightColor : "transparent"
        }

        contentItem: Text {
            text: addConversionButton.text
            font{
                family: fontFamily
                pixelSize: coordinateConversionWindow.fontSize * scaleFactor
            }
            color: textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
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

                delegate: Button{
                    id: addConversionOptionButton
                    text: modelData
                    enabled: text !== inputModeButton.text
                    opacity: enabled ? 1.0 : 0.5
                    anchors{
                        left: parent.left
                    }

                    contentItem: Text {
                        text: addConversionOptionButton.text.toUpperCase()
                        font{
                            family: fontFamily
                            pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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

        Text {
            anchors.fill: zoomToButton
            text: "Go"
            color: textColor
            font {
                family: fontFamily
                pixelSize: coordinateConversionWindow.fontSize * scaleFactor
                bold: true
            }
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
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
            anchors.centerIn: parent
            sourceSize.height: parent.width
            height: sourceSize.height
            source: "images/flash.png"
        }

        onClicked: {
            if (geoView === null)
                return;

            var screenPos = coordinateConvController.screenCoordinate(geoView.width, geoView.height);
            if (screenPos.x === 0.0 && screenPos.y === 0.0)
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
                width: 1 * scaleFactor
            }
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            sourceSize.height: parent.width
            height: sourceSize.height
            opacity: editCoordinateButton.checked ? 1.0 : 0.5
            source: "images/edit.png"
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
                width: 1 * scaleFactor
            }
        }

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            sourceSize.height: parent.width
            height: sourceSize.height
            opacity: captureModeButton.checked ? 1.0 : 0.5
            source: "images/directionsto_dark.png"
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
                    pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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
                    pixelSize: coordinateConversionWindow.fontSize * scaleFactor
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
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: editMenuButton
                    sourceSize.height: editMenuButton.width
                    height: sourceSize.height
                    source: "images/menu.png"
                }

                onClicked: {
                    editMenu.open();
                }

                Menu {
                    id: editMenu
                    visible: false

                    Column {
                        anchors.margins: 10 * scaleFactor
                        spacing: 10 * scaleFactor
                        Label {
                            text: "Delete"
                            font {
                                family: fontFamily
                                pixelSize: coordinateConversionWindow.fontSize * scaleFactor
                            }
                            color: textColor

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    editMenu.close();
                                    coordinateConvController.removeCoordinateFormat(name);
                                }
                            }
                        }

                        Label {
                            text: "Copy"
                            font {
                                family: fontFamily
                                pixelSize: coordinateConversionWindow.fontSize * scaleFactor
                            }
                            color: textColor

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    editMenu.close();
                                    coordinateConvController.copyToClipboard(notation);
                                }
                            }
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
        height: 16 * scaleFactor
        width: height
        radius: height
        color: highlightColor
        border {
            color: backgroundColor
            width: 1 * scaleFactor
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
