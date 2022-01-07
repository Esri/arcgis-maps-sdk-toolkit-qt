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

//    // debug icon missing
//    Menu {
//        visible: true
//        MenuItem {
//            id: iconimg
//            icon.height: 50
//            icon.width: 50
//            leftPadding: 0
//            rightPadding: 0
//            topPadding: 0
//            bottomPadding: 0
//            icon.source : "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/organization-24.svg"
//        }
//        MenuItem {
//            text: "testimage"
//        }
//    }
//    // enddebug

    RowLayout {
        //anchors.fill: parent

        ToolBar {
            id: levelFilterMenu
            visible: true
            Action {
                id: close
                icon.source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/x.svg"
                onTriggered: floorFilterMenu1.close()
            }

            Action {
                id: facility
                icon.source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/organization-24.svg"
                onTriggered: facilityFilterMenu.visible ? facilityFilterMenu.close() : facilityFilterMenu.open()
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

            ToolButton {
                Layout.fillWidth: true
                action: facility
                icon.color: "transparent"
            }
            }

            Component.onCompleted: console.log("1: ", x, y, width, height);
        }

        Menu {
            id: facilityFilterMenu
            visible: true
            // debug: need to fix the layout. hardcoding different position (otherwise they sit on each other)
            x: 350

            MenuItem {
                contentItem: Item {
                    RowLayout {
                        anchors.fill: parent
                        Image {
                            source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/plus-circle.svg"
                        }

                        ColumnLayout {
                            Text {
                                Layout.fillWidth: parent
                                text: "1"
                            }
                            Text {
                                Layout.fillWidth: parent
                                text: "2"
                            }
                        }

                        Image {
                            source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/plus-circle.svg"
                        }
                    }
                }
            }
            TextField {
                placeholderText: "Search"
            }

            Repeater {
                model: controller.facilities
                delegate : MenuItem {
                    text: '\u2022 ' + model.name
                }
            }
            Component.onCompleted: console.log("1: ", x, y, width, height);
        }
    Component.onCompleted: console.log("grid: ", x, y, width, height);
    }
    Component.onCompleted: console.log("item: ", x, y, width, height);
}
