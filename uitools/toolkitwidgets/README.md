[![ArcGIS Maps SDK for Qt Toolkit API reference](https://img.shields.io/badge/API_Reference-purple)](https://developers.arcgis.com/qt/latest/toolkit/api-reference/) [![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt)

## ArcGIS Maps SDK - Qt Widgets UI components

These are controls that compliment your MapView or SceneView by adding them to your projects .cpp files. They are components like: AuthenticationView, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for traditional desktop based apps.

You can learn more about the benefits of building apps with the [Qt Widgets UI controls](https://doc.qt.io/qt-6/qtwidgets-index.html). 

## Available components

These are the components available to use:

- **[AuthenticationView - Qt Widgets UI control/Class](docs/AuthenticationView.md)** - A wrapper view that will automatically display the proper authentication view for any of the supported authentication types (OAuth, Token, HTTP Basic, HTTP Digest, SAML, PKI).
- **[BasemapGallery - Qt Widgets UI control/Class](docs/BasemapGallery.md)** - Shows a list of available basemaps by using a Portal or using a user defined collection. User is able to select a basemap, setting it on the current Geomodel.
- **[BookmarksView - Qt Widgets UI control](/docs/BookmarksView.md)**- A control that displays and navigates to bookmarks that have been authored in a map.
- **[CoordinateConversion - Qt Widgets UI control/Class](docs/CoordinateConversion.md)** - A tool that helps convert coordinates from one coordinate system to another.
- **Floor filter** - A tool that shows sites and facilities, and enables toggling the visibility of levels on floor-aware maps and scenes.
- **[NorthArrow - Qt Widgets UI control/Class](docs/NorthArrow.md)** - A control that indicates north, reacting to rotation changes of the map.
- **[OverviewMap - Qt Widgets UI control/Class](docs/OverviewMap.md)** - An inset map that shows the current extent of an associated GeoView in the context of a larger area.

PUT IN A NOTE THAT THERE ARE NOT AS MANY QT WIDGETS UI COMPONENTS AS THERE ARE QT QUICK UI COMPONENTS. HOWEVER, YOU CAN CREATE YOUR OWN VERSION OF THESE CONTROLS USING THE VARIOUS "CONTROLLER" AND ANCILLARY CLASSES AVAILABLE IN THE https://developers.arcgis.com/qt/toolkit/api-reference/cppwidgetsuitools.html

## Options to use the components in your project

There are two options to make use of the Qt Quick UI components depending on how your app development configuration:

### OPTION1: Making use of the toolkitwidgets.pri file

#### Importing the toolkit (toolkitwidgets.pri)

A good way to start using the toolkit is to use one of the template apps which get added to QtCreator when you install the ArcGIS Maps SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.6.0 Qt Widgets App`

- Choose settings to match the platform etc. you are building for

- In the `.pro` file of your new app, add a line to add the library for your QML plugin - for example:

```qmake
include(path/to/toolkitwidgets.pri)
```

#### Using a tool from the toolkit (toolkitwidgets.pri)

Once you have successfully imported the toolkit, you can create individual tools in your own widgets files.

- In your widgets code, create a new instance of the tool you wish to use and add it to your widgets layout. You will also normally need to pass the `GeoView` which the tool is designed to work with:

```cpp
  #include "Esri/ArcGISRuntime/Toolkit/NorthArrow.h"
  ...
  auto northArrow = new Esri::ArcGISRuntime::Toolkit::NorthArrow(this);
  northArrow->setMapView(m_mapView);
  // Add your NorthArrow to your UI here!
  northArrow->show();
```

### OPTION2: CMake based projects 

#### Importing the toolkit (toolkitwidgets.cmake)

A good way to start using the toolkit is to use one of the template apps which get added to QtCreator when you install the ArcGIS Maps SDK for Qt.

- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Maps 200.6.0 Qt Widgets App`

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

Once you have successfully imported the toolkit, you can create individual tools in your own widgets files.

- In your widgets code, create a new instance of the tool you wish to use and add it to your widgets layout. You will also normally need to pass the `GeoView` which the tool is designed to work with:

```cpp
  #include "Esri/ArcGISRuntime/Toolkit/NorthArrow.h"
  ...
  auto northArrow = new Esri::ArcGISRuntime::Toolkit::NorthArrow(this);
  northArrow->setMapView(m_mapView);
  // Add your NorthArrow to your UI here!
  northArrow->show();
```

## Access token requirements

Some of the toolkit components and examples utilize ArcGIS location services which require an access token. Please see the [setup guide](http://links.esri.com/create-an-api-key) for more information.