import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import Esri.ArcGISRuntime.Toolkit 100.13
import Esri.ArcGISRuntime.Toolkit.Controller 100.13

Control {
    id: scaleLine

    property var mapView;

    /*!
  \qmlproperty ScalelineController controller
  \brief
  */
    property var controller: ScalelineController { }

    spacing: 5
    implicitWidth: 100
    implicitHeight: font.pixelSize + spacing

    enum Style {
        Ruler,
        Line
    }

    enum UnitSystem {
        Imperial = 0,
        Metric = 1,
        Dual = 99
    }

    property int unitSystem: Scaleline.UnitSystem.Metric

    contentItem: Column {
        ScalelineImpl {
            //anchors.top: parent
            controller: scaleLine.controller
            unitSystem: unitSystem === unitSystem.Dual ? Scaleline.UnitSystem.Metric : unitSystem
        }
//        ScalelineImpl {
//            //anchors.top: parent
//            controller: scaleLine.controller
//            unitSystem: Scaleline.UnitSystem.Imperial
//            visible: unitSystem === Scaleline.UnitSystem.Dual
//        }
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
