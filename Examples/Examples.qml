/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtGraphicalEffects 1.0
import Esri.ArcGISRuntime 100.5

Rectangle {
    width: 640
    height: 480

    ListModel {
        id: examplesModel

        ListElement {
            description: "PopupView Example"
            source: "PopupExample.qml"
        }        

        ListElement {
            description: "Callout Example"
            source: "CalloutExample.qml"
        }        
    }


    Column {
        id: banner

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 10
        }

        spacing: 5

        Text {
            text: "ArcGIS Runtime Toolkit Examples"
            font {
                pointSize: 36
            }
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "blue"
        }

        ComboBox {
            width: parent.width
            model: examplesModel
            textRole: "description"

            onCurrentIndexChanged: {
                var example = model.get(currentIndex);
                exampleLoader.source = example.source;
            }
        }
    }

    RectangularGlow {
        anchors.fill: exampleLoader
        cornerRadius: 5
        glowRadius: 9
        color: "darkgrey"
    }

    Loader {
        id: exampleLoader

        anchors {
            left: parent.left
            right: parent.right
            top: banner.bottom
            bottom: parent.bottom
            margins: 10
        }
    }
}
