import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.CoordinateConversionTool 1.0

Rectangle {
    height: 800
    width: 600

    CoordinateConversionEngine {
        id: coordinateConversionEngine
        objectName: "coordinateConversionEngine"

        CoordinateConversionOptions {
            name: "Coast Guard"
            outputMode: CoordinateConversionOptions.CoordinateTypeLatLon
            latLonFormat: CoordinateConversionOptions.LatitudeLongitudeFormatDegreesDecimalMinutes
        }

        CoordinateConversionOptions {
            name: "Air Force"
            outputMode: CoordinateConversionOptions.CoordinateTypeUsng
            precision: 7
            addSpaces: true
        }

        CoordinateConversionOptions {
            name: "Squad Alpha"
            outputMode: CoordinateConversionOptions.CoordinateTypeUtm
            utmConversionMode: CoordinateConversionOptions.UtmConversionModeNorthSouthIndicators
            addSpaces: true
        }

        CoordinateConversionOptions {
            name: "Division B"
            outputMode: CoordinateConversionOptions.LatitudeLongitudeFormatDegreesMinutesSeconds
            decimalPlaces: 12
        }
    }

    CoordinateConversionOptions {
        id: options
    }

    property CoordinateConversionTool tool: null
    property CoordinateConversionEngine engine: null
    property bool getFromMapMode: false
    property int buttonWidth: 100
    property int spacingVal: 5

    Connections {
        target: engine

        onResultsChanged: {
            if (getFromMapMode && !coordinateConversionUI.visible) {
                coordinateConversionUI.visible = true;
            }
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 30
        spacing: spacingVal * 2

        Row {
            spacing: spacingVal

            ComboBox {
                id: notationTypeCombo
                width: buttonWidth
                model: options.coordinateTypeNames
                onCurrentIndexChanged: {
                    engine.inputMode = options.stringToCoordinateType(currentText);
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
                    engine.convertNotation(inputNotation.text);
                }
            }
        }

        Repeater {
            model: engine ? engine.results : null
            delegate:
                Row {
                    spacing: spacingVal

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
                            tool.copyToClipboard(notationBox.text);
                            notationTypeCombo.currentIndex = notationTypeCombo.model.indexOf(options.coordinateTypeToString(coordinateType));
                        }
                    }
                }
        }

        Row {
            spacing: spacingVal

            CheckBox {
                id: getFromMap
                text: "Get From Map"

                onCheckedChanged: {
                    getFromMapMode = checked;
                    if (checked) {
                        coordinateConversionUI.visible = false;
                    }
                }
            }

            Button {
                id: convertButton
                text: "Convert"
                enabled: !getFromMap.checked
                onClicked: {
                    engine.convertPoint();
                }
            }

            Button {
                id: close
                text: "Close"

                onClicked: {
                    coordinateConversionUI.visible = false;
                }
            }
        }
    }
}
