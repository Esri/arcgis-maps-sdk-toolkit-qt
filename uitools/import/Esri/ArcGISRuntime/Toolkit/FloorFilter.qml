/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
import  Esri.ArcGISRuntime.Toolkit.Controller 100.14
import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
/*!
  \qmltype FloorFilter
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \since 100.14
  \brief Allows to display and filter the available floor aware layers in the current \c GeoModel.
*/

Item {
    id: floorFilter
    property var geoView;

    property FloorFilterController controller : FloorFilterController { }

    anchors.bottom: parent.bottom
    anchors.left: parent.left

    Binding {
        target : controller
        property : "geoView"
        value : floorFilter.geoView
    }

    RowLayout {
        //anchors.fill: parent
        ColumnLayout {

            ToolBar {
                id: levelFilterMenu
                visible: true


                Action {
                    id: close
                    icon.source: "images/x.svg"
                    onTriggered: levelFilterMenu.visible = false
                }



                ColumnLayout {


                ToolButton {
                    Layout.fillWidth: true
                    action: close
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                Repeater {
                    model: controller.floors
                    delegate: ToolButton {
                        Layout.fillWidth: true
                        text: model.name
                    }
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }


                }
            }

            ToolBar {

                Action {
                    id: facility
                    icon.source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/organization-24.svg"
                    onTriggered: facilityFilterMenu.visible ? facilityFilterMenu.visible = false : facilityFilterMenu.visible = true
                }

                ToolButton {
                    Layout.fillWidth: true
                    action: facility
                    icon.color: "transparent"
                }
            }
            Component.onCompleted: console.log(width, height)
        }

        ColumnLayout {
            id: facilityFilterMenu
            Layout.alignment: Qt.AlignBottom

            Item {
                height: childrenRect.height
                width: childrenRect.width
                Component.onCompleted: console.log("item: ", width, height);
                ColumnLayout {

                    RowLayout {

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            width: leftChevronImg.width + 10
                            border.color: "black"
                            Image {
                                Component.onCompleted: console.log("image: ", width, height);
                                anchors.centerIn: parent
                                id: leftChevronImg
                                source: "images/chevron-left.svg"
                            }
                        }

                        ColumnLayout {
                            Component.onCompleted: console.log("columns: ", width, height);
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Item {
                            width: 150
                            height: childrenRect.height + 10
                            Text {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                text: controller.selectedFacilityId
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                        Item {
                            width: 150
                            height: childrenRect.height + 10
                            Text {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                horizontalAlignment: Text.AlignHCenter
                                text: controller.selectedSiteId
                            }
                        }
                        }

                        Image {
                            id: closeImg
                            source: "images/x.svg"
                        }

                    }

                    Item {
                        width: childrenRect.width
                        height: childrenRect.height

                        Image {
                            id: searchImg
                            sourceSize.width: 32
                            sourceSize.height: 32
                            source: "images/search.svg"
                        }

                        TextField {
                            Layout.fillWidth: true
                            x: searchImg.width
                            placeholderText: "Search"
                            onTextChanged: controller.filterFacilities(text)
                        }
                    }
                }
            }

            ListView {
                visible: true
                implicitHeight: contentHeight
                implicitWidth: contentWidth
                x: 350
                model: controller.filteredFacilities
                delegate: ItemDelegate {
                    text: '\u2022 ' + model.name
                    onClicked: {
                        facilityFilterMenu.visible = false;
                        levelFilterMenu.visible = true;
                    }
                }
            }

        }

    //Component.onCompleted: console.log("grid: ", x, y, width, height);
    }


}
