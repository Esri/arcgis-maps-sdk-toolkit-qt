
/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
import QtQuick 2.12
import Esri.ArcGISRuntime.Toolkit.Controller 100.13

/*!
   \qmltype OverviewMap
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRutime 100.12
   \ingroup ArcGISQtToolkitUiQmlViews
   \brief Defines a small "overview" (or "inset") map displaying a representation of the current viewpoint of the given GeoView.
   For MapViews, the current viewpoint will be represented by a polygon displaying the visible area of the MapView. For SceneViews,
   the current viewpoint will be represented by a reticle (crosshairs) displaying the viewpoint center.
 */

Item {
    id: overviewMap

    /*!
      \qmlproperty OverviewMapController controller.
      \brief The controller handles binding logic between the OverviewMap and
      the \c GeoView where applicable.
     */
    property var controller: OverviewMapController { }

    /*!
       \qmlproperty GeoView geoView
       \brief The \c GeoView for this tool. Should be a \c SceneView or a \c MapView.
     */
    property var geoView;

    /*!
      \qmlproperty real scaleFactor
      \brief Sets the amount to scale the OverviewMap compared to the geoView.
     */
    property real scaleFactor: 25;

    implicitWidth: 300

    implicitHeight: 200

    children: [
        controller.insetView
    ]

    Binding {
        target: controller
        property: "scaleFactor"
        value: scaleFactor
    }

    Binding {
        target: overviewMap
        property: "scaleFactor"
        value: controller.scaleFactor
    }

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    Binding {
        target: controller.insetView.anchors
        property: "fill"
        value: overviewMap
    }
}
