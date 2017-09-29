import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.CppApi 100.2

/*!
    \qmltype CoordinateConversion
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.2
    \brief A window to do coordinate conversion.
*/

Rectangle {
    id: coordinateConversionWindow

    property real scaleFactor: System.displayScaleFactor
    property int buttonWidth: 100 * scaleFactor
    property int spacingValue: 5 * scaleFactor
    property int imageWidth: 36 * scaleFactor
    property int fontSize: 12 * scaleFactor

    CoordinateConversionController {
        id: coordinateConvController
        objectName: "coordinateConversionController"
    }

    ScrollView {
        id: scrollView
        anchors {
            top: parent.top
            bottom: bottomRect.top
            horizontalCenter: parent.horizontalCenter
        }

        anchors.margins: 10 * scaleFactor

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: spacingValue

            Row {
                spacing: spacingValue

                ComboBox {
                    id: notationTypeCombo
                    width: buttonWidth
                    anchors.verticalCenter: parent.verticalCenter
                    model: CoordinateConversionOptions.coordinateTypeNames
                    onCurrentIndexChanged: {
                        coordinateConvController.inputMode = CoordinateConversionOptions.stringToCoordinateType(currentText);
                    }
                }

                TextField {
                    id: inputNotation
                    width: coordinateConversionWindow.width * 0.7
                    color: "black"
                    font.pixelSize: fontSize
                }

                RoundButton {
                    id: convertButton
                    width: 32 * scaleFactor
                    height: 32 * scaleFactor

                    background: Rectangle {
                        color: Material.accent
                        radius: convertButton.radius

                        Image {
                            anchors.centerIn: parent
                            sourceSize.width: 26 * scaleFactor
                            height: width
                            source: "images/convert.png"
                        }
                    }

                    onClicked: {
                        coordinateConvController.convertNotation(inputNotation.text);
                    }
                }
            }

            Repeater {
                id: repeater
                model: coordinateConvController.results

                delegate: Row {
                    spacing: spacingValue

                    Label {
                        id: repeaterLabel
                        anchors.verticalCenter: parent.verticalCenter
                        width: buttonWidth
                        text: name
                        color: "black"
                    }

                    TextField {
                        id: notationBox
                        anchors.verticalCenter: parent.verticalCenter
                        text: notation
                        width: coordinateConversionWindow.width * 0.7
                        color: "black"
                        font.pixelSize: fontSize
                    }

                    RoundButton {
                        id: copyButton
                        width: 32 * scaleFactor
                        height: 32 * scaleFactor

                        background: Rectangle {
                            color: Material.primary
                            radius: copyButton.radius

                            Image {
                                anchors.centerIn: parent
                                sourceSize.width: 26 * scaleFactor
                                height: width
                                source: "images/editcopy.png"
                            }
                        }

                        onClicked: {
                            coordinateConvController.copyToClipboard(notationBox.text);
                            notationTypeCombo.currentIndex = notationTypeCombo.model.indexOf(CoordinateConversionOptions.coordinateTypeToString(coordinateType));
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: bottomRect

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: coordinateConversionWindow.right
            topMargin: 5 * scaleFactor
        }
        height: 40 * scaleFactor
        color: Material.primary

        CheckBox {
            id: getFromMap
            anchors {
                right: clear.left
                bottom: parent.bottom
                top: parent.top
                rightMargin: 10 * scaleFactor
            }

            onCheckedChanged: {
                coordinateConvController.runConversion = checked;
            }
        }

        Button {
            id: useLocationFromMap
            width: 32 * scaleFactor
            height: 32 * scaleFactor
            checkable: true
            checked: false

            anchors {
                right: clear.left
                bottom: parent.bottom
                top: parent.top
                rightMargin: 10 * scaleFactor
            }

            background: Rectangle {
                anchors.fill: this
                color: Material.primary
            }

            Image {
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                sourceSize.height: parent.width
                height: sourceSize.height
                source: useLocationFromMap.checked ? "images/directionsto_dark.png" : "images/directionsto_light.png"
            }

            onCheckedChanged: {
                coordinateConvController.runConversion = checked;
            }
        }

        Button {
            id: clear
            width: 32 * scaleFactor
            height: 32 * scaleFactor

            anchors {
                right: parent.right
                rightMargin: (coordinateConversionWindow.width - (coordinateConversionWindow.width * 0.7 + buttonWidth + spacingValue * 4 + clear.width)) * scaleFactor
                verticalCenter: parent.verticalCenter
            }

            background: Rectangle {
                anchors.fill: this
                color: Material.primary
            }

            Image {
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                sourceSize.height: parent.width
                height: sourceSize.height
                source: "images/trash.png"
            }

            onClicked: {
                coordinateConvController.clearResults();
            }
        }
    }
}
