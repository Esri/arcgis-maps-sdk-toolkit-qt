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

import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Shapes

import Esri.ArcGISRuntime.Toolkit
import Esri.ArcGISRuntime.Toolkit.Controller

/*!
   \qmltype Scalebar
   \ingroup ArcGISQtToolkit
   \ingroup ArcGISQtToolkitUiQmlViews
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.13
   \brief A Scalebar control that shows an accurate distance that can be used to visually gauge distances on a map view. The
    measurement system used is controlled by \l{unitSystem}. The units used will be appropriate to the
    distance being shown e.g. \c{km} for long distances and \c{m} for shorter distances.
 */
Control {
    id: scalebar

    /*!
     \qmlproperty MapView mapView
     \brief The mapView in which the displayed distance and width
     of this Scalebar are calculated from.
     */
    property var mapView;

    /*!
      \internal
    */
    property var controller: ScalebarController { }

    spacing: 5
    implicitWidth: 100
    implicitHeight: font.pixelSize + spacing

    // TODO: Implement and remove `\internal`.
    /*
      \internal
      \brief The Styles of the Scalebar.

      Valid options are:

      \value Style.Ruler The Scalebar is displayed as a ruler.
      \value Style.Line The Scalebar is displayed as a line.

      The default is \c Style.Line.
     */
    enum Style {
        Ruler,
        Line
    }

    // TODO: Allow for dual display.
    // Future maintainers, please note that
    // the values map to the `UnitSystem` enumeration.
    // Which is why `Dual` is a high value as not to conflict.
    /*
      \brief The Units of the Scalebar.

      Valid options are:

      \value UnitSystem.Imperial Units are in miles and feet.
      \value UnitSystem.Metric Units are in kilometers and meters.

      The default is \c UnitSystem.Metric.
     */
    enum UnitSystem {
        Imperial = 0,
        Metric = 1,
        Dual = 99
    }

    /*!
     \qmlproperty UnitSystem unitSystem
      \brief The display units of the Scalebar, can be metric, imperial.
             The default is \c UnitSystem.Metric.
     */
    property int unitSystem: Scalebar.UnitSystem.Metric

    contentItem: Column {
        ScalebarImpl {
            spacing: scalebar.spacing
            implicitWidth: scalebar.implicitWidth
            implicitHeight: scalebar.implicitHeight
            font: scalebar.font
            controller: scalebar.controller
            unitSystem: unitSystem === unitSystem.Dual ? Scalebar.UnitSystem.Metric : unitSystem
        }
    }

    Binding {
        restoreMode: Binding.RestoreNone
        target: controller
        property: "mapView"
        value: mapView
    }

    onMapViewChanged: {
        if (controller) {
            controller.mapView = mapView;
        }
    }
}
