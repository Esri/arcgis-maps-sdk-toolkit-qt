import QtQuick 2.0
import QtQuick.Controls 2.2
//import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.CppApi 100.2

Rectangle {
    id: coordinateConversionWindow

    property real scaleFactor: System.displayScaleFactor
    property int buttonWidth: 100 * scaleFactor
    property int spacingValue: 5 * scaleFactor
    property int imageWidth: 36 * scaleFactor
    property int fontSize: 12 * scaleFactor
    property double labelWidth: 0.0

    signal clearClicked();

    CoordinateConversionController {
        id: controller
        objectName: "coordinateConversionController"
    }

    ScrollView {
        id: scrollView
        anchors {
            top: parent.top
            bottom: bottomRect.top
            right: parent.right
            left: parent.left
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
                    model: CoordinateConversionOptions.coordinateTypeNames
                    onCurrentIndexChanged: {
                        controller.inputMode = CoordinateConversionOptions.stringToCoordinateType(currentText);
                    }
                }

                TextField {
                    id: inputNotation
                    width: coordinateConversionWindow.width * 0.7
                }

                Button {
                    id: repeaterButton
                    width: 32 * scaleFactor
                    height: 32 * scaleFactor

                    background: Rectangle {
                        anchors.fill: this
                        color: Material.primary
                    }

                    Image {
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        sourceSize.height: parent.width
                        height: sourceSize.height
                        source: "qrc:/Resources/icons/xhdpi/ic_menu_previousslide_dark_d.png"
                        rotation: 180
                    }

                    onClicked: {
//                        controller.copyToClipboard(notationBox.text);
//                        notationTypeCombo.currentIndex = notationTypeCombo.model.indexOf(CoordinateConversionOptions.coordinateTypeToString(coordinateType));
                    }
                }

            }

            Repeater {
                id: repeater
                model: controller.results

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

                    Button {
                        id: repeaterButton
                        width: 32 * scaleFactor
                        height: 32 * scaleFactor

                        background: Rectangle {
                            anchors.fill: this
                            color: Material.primary
                        }

                        Image {
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                            sourceSize.height: parent.width
                            height: sourceSize.height
                            source: "qrc:/Resources/icons/xhdpi/editcopy32_d_gray.png"
                        }

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
                controller.runConversion = checked;
            }
        }

        Button {
            id: clear
            width: 32 * scaleFactor
            height: 32 * scaleFactor

            anchors {
                right: parent.right
                rightMargin: coordinateConversionWindow.width - ((coordinateConversionWindow.width * 0.7) + buttonWidth + (spacingValue * 2) + (32 * scaleFactor) + (10 * scaleFactor))
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
                source: "qrc:/Resources/icons/xhdpi/ic_menu_trash_dark_d.png"
            }

            onClicked: {
                clearClicked();
            }
        }
    }
}
