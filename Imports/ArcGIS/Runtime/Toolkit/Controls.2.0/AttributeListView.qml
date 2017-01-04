/*******************************************************************************
 * Copyright 2012-2016 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISExtras 1.1

/*!
    \qmltype AttributeListModel
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri::ArcGISRutime::Toolkit 2.0
    \brief A view for displaying and editing attributes in an AttributeListModel from a Feature or a Graphic.

    Although edits for Features can be made inside of the view, to update the Feature with the changes, its FeatureTable must call the updateFeature() method.
    A Graphic's attribute values can be edited inside of the view without any additional steps.
*/

Item {
    id: attributeListView

    /*========================================
         Configurable properties
    ========================================*/

    //default properties
    visible: false
    height: 250 * scaleFactor
    width: height

    /*!
        \brief The color of the title bar of the view.

        The default color is \c "#3F51B5".
    */
    property color barColor: "#3F51B5"

    /*!
        \brief The color of the text and buttons in the title bar.

        The default color is \c "#FFFFFF".
    */
    property color barTextColor: "#FFFFFF"

    /*!
        \brief The background color of the cells in the view.

        The default color is \c "#F5F5F5".
    */
    property color cellColor: "#F5F5F5"

    /*!
        \brief The color of the cell borders in the view.

        The default color is \c "#000000".
    */
    property color cellBorderColor: "#000000"

    /*!
        \brief The height of the rows in the list.

        The default value is \c 20.
    */
    property real rowHeight: 20 * scaleFactor

    /*!
        \brief Whether the view should stay permanently expanded.

        The default value is \c false.
    */
    property bool alwaysExpanded: false

    /*!
        \brief This property holds the model that provides data for the list of attributes.

        When displaying the attributes of a Feature, the Feature must be loaded before using its attributes as a model.
    */
    property var model: null

    /*!
      \qmlsignal AttributeListModel::attributeValueChanged()
      \brief Emitted when an attribute value inside the model has been modified.
     */
    signal attributeValueChanged();

    // internal properties
    /*! \internal */
    property int attributeCount
    /*! \internal */
    property real columnWidth: width / 2
    /*! \internal */
    property bool expanded: false

    // do not allow mouse actions to propagate down
    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
        onWheel: wheel.accepted = true
    }

    Column {
        id: view
        height: attributeListView.height
        width: attributeListView.width
        anchors.fill: parent

        Rectangle {
            id: topBar
            width: columnWidth * 2.50
            height: Qt.platform.os === "ios" || Qt.platform.os === "android" ?  rowHeight * 2.50 : rowHeight * 1.50
            color: attributeListView.barColor
            radius: 1

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

                Image {
                    width: attributeListView.height / 12
                    height: width
                    visible: !alwaysExpanded
                    source: "images/triangle.png"
                    rotation: expanded ? 0 : 180

                    Behavior on rotation {
                        SmoothedAnimation {
                            duration: 650
                        }
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
                                    listView.width = attributeListView.columnWidth * 2 + 50 * scaleFactor;
                                    topBar.width = attributeListView.columnWidth * 2;
                                }

                                expanded = !expanded;
                            }
                        }
                    }
                }


                Image {
                    height: attributeListView.height / 12
                    width: height
                    source: "images/exit.png"

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
                    width: listView.width / 2 - 50 * scaleFactor
                    color: cellColor
                    border.color: cellBorderColor

                    Text {
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignLeft
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

                        // confirm edit
                        Image {
                            height: rowHeight
                            width: 25 * scaleFactor
                            source: "images/check-mark.png"

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

                        // cancel edit
                        Image {
                            source: "images/exit.png"
                            height: rowHeight
                            width: 25 * scaleFactor

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
