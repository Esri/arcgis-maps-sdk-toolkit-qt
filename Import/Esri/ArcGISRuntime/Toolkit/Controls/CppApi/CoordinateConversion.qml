import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.2
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

    /*! \brief internal */
    property real scaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    /*!
      \brief Width of buttons.

      Default value is 100.
     */
    property int buttonWidth: 100

    /*!
      \brief spacing between rows and columns

      Default value is 5.
     */
    property int spacingValue: 5

    /*!
      \brief Font size of text in TextFields.

      Default value is 12.
     */
    property int fontSize: 12

    /*!
      \brief Text color for TextFields and Labels.

      Default value is "black".
     */
    property color textColor: "black"

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
            spacing: spacingValue * scaleFactor

            Row {
                spacing: spacingValue * scaleFactor

                ComboBox {
                    id: notationTypeCombo
                    width: buttonWidth * scaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    model: CoordinateConversionOptions.coordinateTypeNames
                    onCurrentTextChanged: {
                        coordinateConvController.inputMode = CoordinateConversionOptions.stringToCoordinateType(currentText);
                    }
                }

                TextField {
                    id: inputNotation
                    width: coordinateConversionWindow.width * 0.7
                    color: textColor
                    font.pixelSize: fontSize * scaleFactor
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
                    spacing: spacingValue * scaleFactor

                    Label {
                        id: repeaterLabel
                        anchors.verticalCenter: parent.verticalCenter
                        width: buttonWidth * scaleFactor
                        text: name
                        color: textColor
                    }

                    TextField {
                        id: notationBox
                        anchors.verticalCenter: parent.verticalCenter
                        text: notation
                        width: coordinateConversionWindow.width * 0.7
                        color: textColor
                        font.pixelSize: fontSize * scaleFactor
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
                id: img
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                sourceSize.height: parent.width
                height: sourceSize.height
                opacity: useLocationFromMap.checked ? 1.0 : 0.5
                source: "images/directionsto_dark.png"
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
                rightMargin: (coordinateConversionWindow.width - (coordinateConversionWindow.width * 0.7 + (buttonWidth * scaleFactor) + (spacingValue * scaleFactor * 4) + clear.width)) * scaleFactor
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
