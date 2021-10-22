# Authentication View
AuthenticationView provides a dialog for the user to input credentials whenever an authentication challenges is produced by ArcGISRuntime. This dialog is context sensitive and will change based on the type of challenge that is produced by ArcGIS runtime.
+screenshot
OAuth 2.0 uses QWebEngineView. To use OAuth you must add ```QT += webenginewidgets``` to qmake in your ```.pro``` file

### C++ Quick and QML example code
```c++
import Esri.ArcGISRuntime 100.12
import Esri.ArcGISRuntime.Toolkit 100.12
AuthenticationView {

    anchors.centerIn: parent

}
MapView {
    anchors.fill: parent
    
    Map {

        BasemapTopographic {}

        ArcGISMapImageLayer {

            url: "https://sampleserver6.arcgisonline.com/arcgis/rest/services/USA_secure_user1/MapServer"

        }

    }

}
```

### QWidget example code
+add code
```
```

# North Arrow
The NorthArrow tool is a UI component that always points North with reference to the orientation of your SceneView/MapView.
Double click it and will reset the map to the original orientation.
+screenshot

### C++ Quick and QML example code
```c++
import Esri.ArcGISRuntime 100.12
import Esri.ArcGISRuntime.Toolkit 100.12

MapView {
    id: mapView

    Map {
      BasemapTopographic {}
    }

    // A visible compass is now embedded in the mapView and hooked up!
    NorthArrow {
        id: arrow
        width: 32
        height: 32
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        geoView: mapView
    }
}
```

### QWidget example code
```c++
  #include "Esri/ArcGISRuntime/Toolkit/NorthArrow.h"
  ...
  auto northArrow = new Esri::ArcGISRuntime::Toolkit::NorthArrow(this);
  northArrow->setMapView(m_mapView);
  // Add your NorthArrow to your UI here!
  northArrow->show();
```

# Time Slider
