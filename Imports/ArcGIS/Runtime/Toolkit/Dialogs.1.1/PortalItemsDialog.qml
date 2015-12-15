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

import QtQuick 2.2
import QtQuick.Controls 1.1

import ArcGIS.Runtime 10.27
import ArcGIS.Runtime.Toolkit.Controls 1.1

import QtQuick.Dialogs 1.2

Dialog {
    id: dialog

    property Portal portal
    property alias view: portalItemsView
    property alias query: portalItemsView.query
    property alias itemInfo: portalItemsView.currentItem

    modality: Qt.ApplicationModal
    visible: false

    width: 500
    height: 500

    standardButtons: StandardButton.Ok + StandardButton.Cancel

    Component.onCompleted: {
        portalItemsView.refresh();
    }

    Item {
        anchors.fill: parent

        Item {
            height: dialog.height
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                leftMargin: -12
                rightMargin: -12
                topMargin: -12
            }

            PortalItemsView {
                id: portalItemsView
                portal: dialog.portal
                highlightFollowsCurrentItem: true

                anchors {
                    fill: parent
                    margins: 10
                    bottomMargin: 50
                }

                onDoubleClicked: {
                    dialog.accepted();
                    dialog.close();
                }
            }
        }
    }
}
