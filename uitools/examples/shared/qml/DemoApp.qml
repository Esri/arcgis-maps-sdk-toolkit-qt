// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13
import "tools.js" as T

Item {
    Drawer {
        id: drawer
        height: parent.height
        implicitWidth: 300
        clip: true
        dragMargin: 0
        ListView {
            id: viewList
            anchors.fill: parent
            model: ListModel {
                id: listModel
                Component.onCompleted: {
                    for (let tool of T.TOOLS) {
                        listModel.append(tool);
                    }
                    viewList.currentIndex = 0;
                }
            }
            delegate: ItemDelegate {
                text: name
                highlighted: ListView.isCurrentItem
                width: parent.width
                onClicked: {
                    viewList.currentIndex = index;
                    drawer.close();
                }
            }
        }
    }

    Connections {
        target: content.item
        function onShowToolsButtonPressed() {
            drawer.open()
        }
    }

    Loader {
        id: content
        anchors.fill: parent
        focus: true
        source: viewList.currentIndex >= 0 ? listModel.get(viewList.currentIndex).url : ""
        onLoaded: {item.handlesOwnAuthentication = listModel.get(viewList.currentIndex).handlesOwnAuthentication ?? false;}
    }
}
