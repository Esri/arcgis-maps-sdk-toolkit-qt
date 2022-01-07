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

    Binding {
        target : controller
        property : "geoView"
        value : floorFilter.geoView
    }

    //debug icon missing
    Menu {
        visible: true
        MenuItem {
            id: iconimg
            icon.height: 50
            icon.width: 50
            leftPadding: 0
            rightPadding: 0
            topPadding: 0
            bottomPadding: 0
            icon.source : "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/organization-24.svg"
        }
        MenuItem {
            text: "testimage"
        }
        Component.onCompleted: console.log(iconimg.icon.height)
    }


    GridLayout {
        columns: 2
        columnSpacing: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Menu {
            id: floorFilterMenu1
            visible: true
            Action {
                id: close
                text: "close"
                onTriggered: floorFilterMenu1.close()
            }

            Action {
                id: home
                text: "home"
                icon.source: "qrc:/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/organization-24.svg"
            }


            MenuItem {
                action: close
            }

            MenuSeparator {}

            Repeater {
                model: controller.floors
                delegate: MenuItem {
                    text: model.name
                }
            }

            MenuSeparator {}

            MenuItem {
                action: home
                icon.color: "transparent"
            }
        }
    }
}
