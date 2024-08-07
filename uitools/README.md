## Introduction

This toolkit is an open source resource intended for use with the ArcGIS Maps
SDK for Qt. It provides ready made UI components to complement mapping and GIS
applications. Because it is open source you are encouraged to modify these tools
and add your own.

In the [API Documentation](https://developers.arcgis.com/qt/toolkit/api-reference/), it can be found a visual representation for each of the components in the below table. A set up example code is also available in the docs to help in quickly utilize the tools.

|          Tool         | [C++ (QtQuick)](https://developers.arcgis.com/qt/toolkit/api-reference/cppquickuitools.html) |            [QtWidgets](https://developers.arcgis.com/qt/toolkit/api-reference/cppwidgetsuitools.html)            |
|:---------------------:|:--------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------:|
|  Authentication view  |    [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-authenticationview.html)   |  [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-authenticationview.html)  |
|    Basemap gallery    |      [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-basemapgallery.html)     |    [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-basemapgallery.html)    |
|     Bookmarks view    |                                               ✔                                              |                                                         ✔                                                        |
|        Callout        |         [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-callout.html)         |                                                                                                                  |
| Coordinate conversion |   [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-coordinateconversion.html)  | [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-coordinateconversion.html) |
|      Floor Filter     |       [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-floorfilter.html)       |      [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-floorfilter.html)     |
|      North arrow      |        [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-northarrow.html)       |      [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-northarrow.html)      |
|      Overview map     |       [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-overviewmap.html)       |      [✔](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-overviewmap.html)     |
|    Popup stack view   |      [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupstackview.html)     |                                                                                                                  |
|       Popup view      |        [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupview.html)        |                                                                                                                  |
|        Scalebar       |         [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-scalebar.html)        |                                                                                                                  |
|      Search view      |        [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-searchview.html)       |                                                                                                                  |
|      Time slider      |        [✔](https://developers.arcgis.com/qt/toolkit/api-reference/qml-timeslider.html)       |                                                                                                                  |
| Utility Network Trace |                                               ✔                                              |                                                                                                                  |

## Showcase - Demo app

In the [examples](examples) folder, there can be found three demo applications projects to be built that showcase the tools implemented.

## Structure

This repo contains 2 toolkit projects, each intended for use with a different
flavor of the ArcGIS Maps SDK for Qt:

- toolkitcpp (for use with C++ apps which use a QtQuick UI)
- toolkitwidgets (for use with C++ apps which use a QWidgets UI)

You need to clone this repository locally to incorporate it into your own
project.

## toolkitcpp.cmake (C++ QtQuick)

#### Importing the toolkit (toolkitcpp.cmake)
A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Runime SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.4.0 Qt Quick C++ App`
- Choose settings to match the platform etc. you are building for
- Copy the `toolkitcpp` subdirectory into your project's directory
```bash
cp -r /path/to/toolkit/uitools/toolkitcpp /path/to/project
```
- The following code block is pregenerated in the `CMakeLists.txt` of your template app, uncomment the `add_subdirectory` and `target_link_libraries` commands:
  ```CMake
    # To integrate the toolkit, copy the `toolkitcpp` subdirectory from the toolkit
    # into your project's directory. Then uncomment the following lines to add it to your project.
    # See https://github.com/Esri/arcgis-maps-sdk-toolkit-qt for details
    # add_subdirectory(toolkitcpp)
    # target_link_libraries(${PROJECT_NAME} PRIVATE libtoolkitcpp)
  ```
- In the `main.cpp` file of your new application, do the following:
  - Include the `register.h` file from the toolkit
  ```cpp
  #include "Esri/ArcGISRuntime/Toolkit/register.h"
  ```
  - Call the `registerComponents` function with your `QQmlApplicationEngine`
  ```cpp
  QQmlApplicationEngine engine; // The engine driving your QML application.
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
  ```
#### Using a tool from the toolkit (toolkitcpp.cmake)

Once you have successfully imported the toolkit, you can access individual tools
in your own QML files.

- Add an import statement for the toolkit:
`import Esri.ArcGISRuntime.Toolkit`
- Declare the tool you wish to use. Generally you will also have to supply the
  `GeoView` to the tool you want to work with. For example, to add a `NorthArrow`:

```qml
import Esri.ArcGISRuntime.Toolkit

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

## toolkitcpp.pri (C++ QtQuick)

#### Importing the toolkit (toolkitcpp.pri)

A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Runime SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.5.0 Qt Quick C++ App`
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
`import Esri.ArcGISRuntime.Toolkit`
- declare the tool you wish to use. Generally you will also have to supply the
  `GeoView` etc. you wish the tool to work with. For example, to add a `NorthArrow`:

```qml
import Esri.ArcGISRuntime.Toolkit

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
get added to QtCreator when you install the ArcGIS Maps SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.5.0 Qt Widgets App`
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

## toolkitwidgets.cmake (QtWidgets)

#### Importing the toolkit (toolkitwidgets.cmake)

A good way to start using the toolkit is to use one of the template apps which
get added to QtCreator when you install the ArcGIS Maps SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.4.0 Qt Widgets App`
- Choose settings to match the platform etc. you are building for
- Copy the `toolkitwidgets` subdirectory into your project's directory
```bash
cp -r /path/to/toolkit/uitools/toolkitwidgets /path/to/project
```
- The following code block is pregenerated in the `CMakeLists.txt` of your template app, uncomment the `add_subdirectory` and `target_link_libraries` commands:
  ```CMake
    # To integrate the toolkit, copy the `toolkitwidgets` subdirectory from the toolkit
    # into your project's directory. Then uncomment the following lines to add it to your project.
    # See https://github.com/Esri/arcgis-maps-sdk-toolkit-qt for details
    # add_subdirectory(toolkitwidgets)
    # target_link_libraries(${PROJECT_NAME} PRIVATE libtoolkitwidgets)
  ```
  
#### Using a tool from the toolkit (toolkitwidgets.cmake)

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

## Access token requirements

Some of the toolkit components and examples utilize ArcGIS location services which require an access token.
Please see the [setup guide](http://links.esri.com/create-an-api-key) for more information.
