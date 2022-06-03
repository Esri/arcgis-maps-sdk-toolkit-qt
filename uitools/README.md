## Introduction

This toolkit is an open source resource intended for use with the ArcGIS Runtime
SDK for Qt. It provides ready made UI components to complement mapping and GIS
applications. Because it is open source you are encouraged to modify these tools
and add your own.

In the [API Documentation](https://developers.arcgis.com/qt/toolkit/api-reference/), it can be found a visual representation for each of the components in the below table. A set up example code is also available in the docs to help in quickly utilize the tools.

|Tool|[C++ (QtQuick)](https://developers.arcgis.com/qt/toolkit/api-reference/cppquickuitools.html)|[QML](https://developers.arcgis.com/qt/toolkit/api-reference/qmluitools.html)|[QtWidgets](https://developers.arcgis.com/qt/toolkit/api-reference/cppwidgetsuitools.html)|
|:--:|:--:|:--:|:--:|
|Authentication view|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-authenticationview.html) | [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-authenticationview.html) | [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-authenticationview.html) |
|Basemap gallery|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-basemapgallery.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-basemapgallery.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-basemapgallery.html)|
| Bookmarks view |✔|✔ |✔ |
|Callout|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-callout.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-callout.html)||
|Coordinate conversion|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-coordinateconversion.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-coordinateconversion.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-coordinateconversion.html)|
|Floor Filter|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-floorfilter.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-floorfilter.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-floorfilter.html)|
|North arrow|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-northarrow.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-northarrow.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-northarrow.html)|
|Overview map|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-overviewmap.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-overviewmap.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-overviewmap.html)|
|Popup stack view|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupstackview.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupstackview.html)||
|Popup view|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupview.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupview.html)||
|Scalebar|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-scalebar.html)|||
|Search view|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-searchview.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-searchview.html)||
|Time slider|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-timeslider.html)|[✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-timeslider.html)||

## Showcase - Demo app

In the [examples](examples) folder, there can be found three demo applications projects to be built that showcase the tools implemented.

## Structure

This repo contains 3 toolkit projects, each intended for use with a different
flavor of the ArcGIS Runtime SDK for Qt:

- toolkitqml (for use with QML only apps)
- toolkitcpp (for use with C++ apps which use a QtQuick UI)
- toolkitwidgets (for use with C++ apps which use a QWidgets UI)

You need to clone this repository locally to incorporate it into your own
project.

## toolkitqml.pri (QML QtQuick)

#### Importing the toolkit (toolkitqml.pri)

A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Runime SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.15 Quick QML App`
- choose settings to match the platform etc. you are building for
- in the `.pro` file of your new app, add a line to add the toolkit for your
  QML application. For example:

```qmake
include(path/to/toolkitqml.pri)
```

- in `main.cpp` add a line to import the toolkit registration function.

```cpp
#include "Esri/ArcGISRuntime/Toolkit/register.h"
```

- in `main.cpp` (inside the main function) call the registration function.

```cpp
QQmlApplicationEngine engine; // The engine driving your QML application.
Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
```

#### Using a tool from the toolkit (toolkitqml.pri)

Once you have successfully imported the toolkit, you can access individual tools
in your own QML files.

- add an import statement for the toolkit:
  `import Esri.ArcGISRuntime.Toolkit 100.15`
- declare the tool you wish to use. Generally you will also have to supply the
  `GeoView` etc. you wish the tool to work with. For example, to add a
  `NorthArrow`:

```qml
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit 100.15

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
## toolkitcpp.pri (C++ QtQuick)

#### Importing the toolkit (toolkitcpp.pri)

A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Runime SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.15 Qt Quick C++ App`
- choose settings to match the platform etc. you are building for
- in the `.pro` file of your new app, add a line to add the library for your QML
  plugin - for example:

```qmake
include(path/to/toolkitcpp.pri)
```

- in the Run environment settings for the app, add a new environment variable to
  import the QML module - e.g:

- in `main.cpp` add a line to import the toolkit registration function.

```cpp
#include "Esri/ArcGISRuntime/Toolkit/register.h"
```

- in `main.cpp` (inside the main function) call the registration function.

```cpp
QQmlApplicationEngine engine; // The engine driving your QML application.
Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
```

#### Using a tool from the toolkit (toolkitcpp.pri)

Once you have successfully imported the toolkit, you can access individual tools
in your own QML files.

- add an import statement for the toolkit:
`import Esri.ArcGISRuntime.Toolkit 100.15`
- declare the tool you wish to use. Generally you will also have to supply the
  `GeoView` etc. you wish the tool to work with. For example, to add a `NorthArrow`:

```qml
import Esri.ArcGISRuntime.Toolkit 100.15

MapView {
    id: mapView

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
## toolkitwidgets.pri (QtWidgets)

#### Importing the toolkit (toolkitwidgets.pri)

A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Runtime SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.15 Qt Widgets App`
- choose settings to match the platform etc. you are building for
- in the `.pro` file of your new app, add a line to add the library for your QML
  plugin - for example:

```qmake
include(path/to/toolkitwidgets.pri)
```

#### Using a tool from the toolkit (toolkitwidgets.pri)

Once you have successfully imported the toolkit, you can create individual tools
in your own widgets files.

- in your widgets code, create a new instance of the tool you wish to use and
  add it to your widgets layout. You will also normally need to pass the `GeoView`
  which the tool is designed to work with:

```cpp
  #include "Esri/ArcGISRuntime/Toolkit/NorthArrow.h"
  ...
  auto northArrow = new Esri::ArcGISRuntime::Toolkit::NorthArrow(this);
  northArrow->setMapView(m_mapView);
  // Add your NorthArrow to your UI here!
  northArrow->show();
```

## API Key requirements

Some of the toolkit components and examples utilize ArcGIS Platform services which require an API Key.
Please see the [setup guide](https://developers.arcgis.com/qt/get-started/#3-access-services-and-content-with-an-api-key) for more information.