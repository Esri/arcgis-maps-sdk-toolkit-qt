import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISExtras 1.1

Item {
    id: attributeListView
    height: 300 * scaleFactor
    width: 250 * scaleFactor
    visible: false

    // user configurable properties

    // The color of the title bar of the view. The default color is "#3F51B5".
    property color barColor: "#3F51B5"

    // The color of the text and buttons in the title bar. The default color is "#FFFFFF".
    property color barTextColor: "#FFFFFF"

    // The background color of the cells in the view. The default color is "#F5F5F5".
    property color cellColor: "#F5F5F5"

    // The color of the cell borders in the view. The default color is "#000000".
    property color cellBorderColor: "#000000"

    // The height of the rows. The default height is 20.
    property real rowHeight: 20 * scaleFactor

    // Whether the view should stay permanently expanded. The default value is false.
    property bool alwaysExpanded: false

    // This property holds the model that provides data for the list of attributes.
    // When displaying the attributes of a Feature, the Feature must be loaded before using its attributes as a model.
    property var model: null

    // Emitted when an attributeValue has been modified.
    signal attributeValueChanged();

    // internal properties
    property int attributeCount
    property real columnWidth: width / 2
    property bool expanded: false

    Column {
        id: view
        height: attributeListView.height
        width: attributeListView.width
        anchors.fill: parent

        Rectangle {
            id: topBar
            width: columnWidth
            height: rowHeight
            color: attributeListView.barColor

            // animate on expand and collapse
            Behavior on width {
                SpringAnimation {
                    spring: 3
                    damping: .4
                }
            }

            // title
            Text {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: 5 * scaleFactor
                }
                width: topBar.width / 2
                elide: Text.ElideRight
                color: attributeListView.barTextColor
                renderType: Text.NativeRendering
                text: model !== null ? attributeCount + " Attributes" : "Attributes"
            }

            // controls to exit and expand/collapse view
            Row {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    margins: 3 * scaleFactor
                }

                spacing: 2 * scaleFactor

                Rectangle {
                    color: "transparent"
                    width: attributeListView.height / 20
                    height: width
                    border.color: attributeListView.barTextColor
                    radius: 90
                    visible: !alwaysExpanded

                    Text {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: attributeListView.height / 33
                        color: attributeListView.barTextColor
                        renderType: Text.NativeRendering
                        text: expanded ? "-" : "+"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // ignore if user sets view to always be expanded
                            if (!alwaysExpanded) {

                                // if view is currently expanded, collapse it
                                if (expanded) {
                                    listView.height = 0;
                                    listView.width = 0;
                                    topBar.width = columnWidth;
                                }

                                // otherwise expand
                                else {
                                    listView.height = attributeListView.height - topBar.height;
                                    listView.width = attributeListView.columnWidth * 2 + 80 * scaleFactor;
                                    topBar.width = attributeListView.columnWidth * 2;
                                }

                                expanded = !expanded;
                            }
                        }
                    }
                }

                Rectangle {
                    color: "transparent"
                    width: attributeListView.height / 20
                    height: width
                    border.color: attributeListView.barTextColor
                    radius: 90

                    Text {
                        anchors.centerIn: parent
                        color: attributeListView.barTextColor
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: "x"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            attributeListView.visible = false;
                        }
                    }
                }
            }
        }

        // displays the attribute names on the left column and attribute values on the right
        ListView {
            id: listView
            height: 0
            width: 0
            model: attributeListView.model
            clip: true

            // animate on expand and collapse
            Behavior on height {
                SpringAnimation {
                    spring: 3
                    damping: .4
                }
            }

            Behavior on width {
                SpringAnimation {
                    spring: 3
                    damping: .4
                }
            }

            delegate: Row {

                // used to store old value in case user decides to cancel edits
                property var originalValue

                Rectangle {
                    height: rowHeight
                    width: listView.width / 2 - 80 * scaleFactor
                    color: cellColor
                    border.color: cellBorderColor

                    Text {
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                        text: attributeName
                    }
                }

                TextField {
                    id: textField
                    text: attributeValue ? attributeValue : ""
                    height: rowHeight
                    width: listView.width / 2
                    horizontalAlignment: TextInput.AlignLeft

                    // controls to confirm or cancel edit
                    Row {
                        id: editControls
                        visible: false
                        anchors.right: parent.right

                        Rectangle {
                            height: rowHeight
                            width: 25 * scaleFactor
                            color: cellColor
                            border.color: cellBorderColor

                            Text {
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                renderType: Text.NativeRendering
                                text: "ok"
                            }

                            MouseArea {
                                anchors.fill: parent

                                // if user confirms change, emit attributeValueChanged signal
                                onClicked:  {
                                    attributeValue = textField.text;
                                    editControls.visible = false;
                                    attributeListView.attributeValueChanged();
                                }
                            }
                        }

                        Rectangle {
                            height: rowHeight
                            width: 25 * scaleFactor
                            color: cellColor
                            border.color: cellBorderColor

                            Text {
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                renderType: Text.NativeRendering
                                text: "x"
                            }

                            MouseArea {
                                anchors.fill: parent
                                // if user cancels, return the original value
                                onClicked: {
                                    textField.text = originalValue ? originalValue : "";
                                    editControls.visible = false;
                                }
                            }
                        }
                    }

                    style: TextFieldStyle {
                        background: Rectangle {
                            height: rowHeight
                            width: listView.width / 2
                            color: cellColor
                            border.color: cellBorderColor
                        }

                        renderType: Text.NativeRendering
                    }

                    onActiveFocusChanged: {
                        if(activeFocus) {
                            originalValue = attributeValue
                            editControls.visible = true;
                        }
                    }
                }
            }
        }
    }

    // when model is changed, reset the view
    onModelChanged: {
        if (!alwaysExpanded) {
            expanded = false;
            listView.height = 0;
            listView.width = 0;
            topBar.width = columnWidth;
        }
        else {
            expanded = true;
            listView.height = attributeListView.height - topBar.height;
            listView.width = attributeListView.columnWidth * 2 + 80 * scaleFactor;
            topBar.width = attributeListView.columnWidth * 2;
        }

        if (model !== null) {
            attributeCount = model.rowCount();
            visible = true;
        }
    }
}
