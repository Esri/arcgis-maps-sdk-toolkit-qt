import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.CppApi 100.2

Rectangle {
    id: coordinateConversionWindow
    height: 800
    width: 600

    property real scaleFactor: System.displayScaleFactor

    property int buttonWidth: 100 * scaleFactor
    property int spacingValue: 5 * scaleFactor

    CoordinateConversionController {
        id: controller
        objectName: "coordinateConversionController"
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: 10 * scaleFactor

        Column {
            id: column
            anchors.margins: 30 * scaleFactor
            spacing: spacingValue * 2

            Row {
                spacing: spacingValue

                ComboBox {
                    id: notationTypeCombo
                    width: buttonWidth
                    model: CoordinateConversionOptions.coordinateTypeNames
                    onCurrentIndexChanged: {
                        controller.inputMode = CoordinateConversionOptions.stringToCoordinateType(currentText);
                    }
                }

                TextField {
                    id: inputNotation
                    width: buttonWidth * 3
                }

                Button {
                    text: "Convert"
                    enabled: inputNotation.text !== ""
                    onClicked: {
                        controller.convertNotation(inputNotation.text);
                    }
                }
            }

            Repeater {
                model: controller.results
                delegate:
                    Row {
                    spacing: spacingValue

                    Label {
                        width: buttonWidth
                        text: name
                    }

                    TextField {
                        id: notationBox
                        text: notation
                        width: buttonWidth * 3
                    }

                    Button {
                        text: "Copy to clipboard"
                        onClicked: {
                            controller.copyToClipboard(notationBox.text);
                            notationTypeCombo.currentIndex = notationTypeCombo.model.indexOf(CoordinateConversionOptions.coordinateTypeToString(coordinateType));
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: scrollView.right
            topMargin: 5 * scaleFactor
            bottomMargin: 5 * scaleFactor
            leftMargin: 10 * scaleFactor
            rightMargin: 30 * scaleFactor
        }
        height: 30 * scaleFactor

        CheckBox {
            id: getFromMap
            anchors {
                left: parent.left
                bottom: parent.bottom
                top: parent.top
            }

            text: "Get From Map"

            onCheckedChanged: {
                controller.runConversion = checked;
            }
        }

        Button {
            id: close
            anchors {
                right: parent.right
                bottom: parent.bottom
                top: parent.top
                rightMargin: 25 * scaleFactor
            }

            text: "Close"

            onClicked: {
                coordinateConversionWindow.visible = false;
            }
        }
    }
}
