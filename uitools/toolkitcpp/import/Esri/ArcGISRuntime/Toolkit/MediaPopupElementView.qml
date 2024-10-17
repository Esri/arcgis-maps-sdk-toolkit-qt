/*******************************************************************************
 *  Copyright 2012-2024 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQuick.Controls

Item {

    /*!
      \qmlproperty MediaPopupElementView controller
      \brief The Controller handles reading from the MediaPopupElement.

      \sa Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController
    */
    property var controller: null

    implicitHeight: childrenRect.height
    width: elementsView.width

    Label {
        text: controller.title !== "" ? controller.title : "Media"
        wrapMode: Text.WordWrap
        width: parent.width
    }
}
