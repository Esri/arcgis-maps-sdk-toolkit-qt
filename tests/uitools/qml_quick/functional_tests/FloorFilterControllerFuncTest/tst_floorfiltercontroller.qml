import QtQuick 2.0
import QtTest 1.0

import Esri.ArcGISRuntime 100.14
import Esri.ArcGISRuntime.Toolkit 100.14
import Esri.ArcGISRuntime.Toolkit.Controller 100.14

TestCase {
    name: "FloorFilterControllerUnitTest"

    MapView {
        id: view
        Map {
            id: map
            item: PortalItem {
                itemId: "f133a698536f44c8884ad81f80b6cfc7"
                portal: Portal {
                    Component.onCompleted: load()
                    onErrorChanged: {
                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
                            console.log(error.message);
                        }
                    }
                    url: "https://www.arcgis.com"
                }
            }
        }
    }

}