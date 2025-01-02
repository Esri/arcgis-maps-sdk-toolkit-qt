[![ArcGIS Maps SDK for Qt Toolkit API reference](https://img.shields.io/badge/API_Reference-purple)](https://developers.arcgis.com/qt/latest/toolkit/api-reference/) [![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt)

## ArcGIS Maps SDK - Qt Quick UI components

These are controls that compliment your `MapView` or `SceneView` by adding them to your Qt project's .qml files (QML based UI development). They are components like: AuthenticationView, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for device based apps like phones and tablets.

You can learn more about the benefits of building apps with the [Qt Quick UI controls](https://doc.qt.io/qt-6/qtquickcontrols-index.html) from the Qt Group. 

## Available components

These are the **Qt Quick UI components/QML Type** available to use:

- **[AuthenticationView](docs/AuthenticationView.md)** - A wrapper view that will automatically display the proper authentication view for any of the supported authentication types (OAuth, Token, HTTP Basic, HTTP Digest, SAML, PKI).
- **[BasemapGallery](docs/BasemapGallery.md)** - Shows a list of available basemaps by using a Portal or using a user defined collection. User is able to select a basemap, setting it on the current GeoModel.
- **[BookmarksView](docs/BookmarksView.md)**- A control that displays and navigates to bookmarks that have been authored in a map.
- **[Callout](docs/Callout.md)** - A view for displaying information at a geographic location.
- **[CoordinateConversion](docs/CoordinateConversion.md)** - A tool that helps convert coordinates from one coordinate system to another.
- **[FloorFilter](docs/FloorFilter.md)** - A tool that shows sites and facilities, and enables toggling the visibility of levels on floor-aware maps and scenes.
- **[NorthArrow](docs/NorthArrow.md)** - A control that indicates north, reacting to rotation changes of the map.
- **[OverviewMap](docs/OverviewMap.md)** - An inset map that shows the current extent of an associated GeoView in the context of a larger area.
- **[PopupStackView](docs/PopupStackView.md)** - A view that displays multiple PopupViews, typically used when multiple map element were tapped on.
- **[PopupView](docs/PopupView.md)** - A view that displays feature attributes and attachments.
- **[Scalebar](docs/Scalebar.md)** - A tool that indicates the proportions of a map to the actual geographic area so users can visually gauge distances on a map view.
- **[SearchView](docs/SearchView.md)** - A tool that submits search queries and shows single or multiple results. Queries can be restricted to the current view extent.
- **[TimeSlider](docs/TimeSlider.md)** - A control for changing the current time extent on a view using a slider that moves between the defined minimum and maximum time values.
- **[UtilityNetworkTrace](docs/UtilityNetworkTrace.md)**- A control that streamlines utility network trace workflows by allowing the user to select a starting point and trace configuration.

## Example steps showing how to use a Qt Quick UI component in your project

 A good way to start using the toolkit is to use one of the ArcGIS Maps SDK for Qt templates to build an app. There are two options to make use of the Qt Quick UI components depending on your Qt Creator app development build configuration (**qmake** or **CMake**). The following steps show an example of adding the NorthArrow control to an app:

**STEP 1:** In Qt Creator choose **File** > **New project** from the menus. 

**STEP 2:** In the **New Project - Qt Creator** dialog. Select **ArcGIS** in the Project template on the left and then choose the **ArcGIS Maps 200.x.0 Qt Quick C++ app** template. Then click the **Choose...** button.

**STEP 3:** Now complete the rest of the dialog wizard options to create a project.

>  **Project Location** ==> **Name:** TestNorthArrow, **Create in:** C:\temp
>
>  **Define Build System** ==> **Build System:** qmake or CMake
>
>  **Define Project Details** ==> **App Description:** Test using a NorthArrow, **3D project:** leave unchecked, **ArcGIS OnlineBasemap:** Imagery, **AccessToken:** see: [Create an API Key](https://developers.arcgis.com/documentation/security-and-authentication/api-key-authentication/tutorials/create-an-api-key/)
>
>  **Kit Selection** ==> Desktop Qt 6.5.6 MSVC2019 64bit (or higher)
>
>  **Project Management** ==> **Add as a subproject to project:** none, **Add to version control:** none

**STEP 4:** Have the project reference the location of the toolkit:

**qmake**

- If you chose the **qmake** build system, edit your apps `TestNorthArrow.pro` file (it was created when you went through the ArcGIS Maps SDK for Qt template wizards), add an `include` statement that points to the path of the `toolkit.pri` file that you have on disk. 

> ```cpp
> ...
> include($$PWD/arcgisruntime.pri)
>
> # Include the path to the toolkit.pri file
> include(C:/arcgis-maps-sdk-toolkit-qt/toolkit/uitools/toolkitcpp.pri)
> ...
> ```  

**CMake**

- If you chose the **CMake** build system, copy the `toolkitcpp` subdirectory into your project's directory. For example you could modify this `bash` script to do the copy of the toolkit directories/files for you:

> ```bash
> cp -r /path/to/toolkit/uitools/toolkitcpp /path/to/project
> ```

- Edit the `CMakeLists.txt` in your Qt project (it was created when you went through the ArcGIS Maps SDK for Qt template wizards). Uncomment the `add_subdirectory` and `target_link_libraries` commands:

> ```CMake
> ...
> # To integrate the toolkit, copy the `toolkitcpp` subdirectory from the toolkit
> # into your project's directory. Then uncomment the following lines to add it to your project.
> # See https://github.com/Esri/arcgis-maps-sdk-toolkit-qt for details
> add_subdirectory(toolkitcpp)
> target_link_libraries(${PROJECT_NAME} PRIVATE libtoolkitcpp)
> ...
> ```

**STEP 5:** In the `main.cpp` file, add an `include` statement near the top of the file to import the toolkit `register.h` file and then later in file call the `Toolkit::registerComponents()` function.

> ```cpp
> ...
> #include <QQmlApplicationEngine>
>
> // Needed for the Qt toolkit
> #include "Esri/ArcGISRuntime/Toolkit/register.h"
> ...
>
> ...
> // Intitialize application view
> QQmlApplicationEngine engine;
>
> // Register the toolkit
> Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
> ...
> ``` 

**STEP 6:** In your apps QML file (for example: TestNorthArrowForm.qml) add an `import` statement for the `Toolkit`. Then later in the file, declare and configure the Qt toolkit component you wish to use. 

  NOTE: In general, you will have to set the `GeoView` property of the Qt toolkit component (and possibly other properties). For example when adding a NorthArrow, your code would look something like this:

> ```qml
> ...
> import Esri.TestNorthArrow
>
> // Needed for the Qt toolkit
> import Esri.ArcGISRuntime.Toolkit
> 
> Item {
>
>    // Create MapQuickView here, and create its Map etc. in C++ code
>    MapView {
>        id: view
>        anchors.fill: parent
>        // set focus to enable keyboard navigation
>        focus: true
>    }
>
>    // Adding the Qt toolkit NorthArrow component via QML.
>    NorthArrow {
>        id: arrow
>        width: 100 // The default is 48 pixels.
>        height: 100 // The default is 48 pixels.
>        anchors {
>            right: parent.right
>            bottom: parent.bottom
>            margins: 30
>        }
>        geoView: view // Set the geoView to the id of the MapView control (ie. view)
>    }
> ...
>```

**STEP 7:** When you run your app, you should now see the UI for the Qt toolkit component in your app. For example:
![Adding code to the main.cpp file](./images/TestNorthArrow.png)

## Access token requirements

Some of the toolkit components and examples utilize ArcGIS location services which require an access token. Please see the [setup guide](http://links.esri.com/create-an-api-key) for more information.

## Example app

In the [examples](../examples) folder, there is a Qt example application project (UitoolExamples.pro) that showcases many of the Qt toolkit components used in a C++ app based on the Qt Quick UI controls.