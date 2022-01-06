import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.14
import Esri.ArcGISRuntime.Toolkit 100.14

DemoPage {
    sceneViewContents: Component {
        SceneView {
            Scene {
                initBasemapStyle: Enums.BasemapStyleArcGISDarkGray
            }
        }
    }

    mapViewContents: Component {
//! [Set up Floor Filter QML]
//        MapView {
//            Map {
//                initBasemapStyle: Enums.BasemapStyleArcGISDarkGray
//            }
//        }
        FloorFilter {
            controller.selectedFacilityId : "hello"
        }
//! [Set up Floor Filter QML]
    }
}
