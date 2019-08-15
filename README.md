# Prototype repo for new qt toolkit

## Introduction
This toolkit is an open source resource inetnded for use with the ArcGIS Runtime SDK for Qt. It provides ready made UI components to complement mapping and GIS applications. Because it is open source you are encouraged to modify these tools and add your own.

## Structure
This repo contains 3 toolkit projects, each intended for use with a different falavor of the ArcGIS Runtime SDK for Qt:
- toolkitqml (for use with QML only apps)
- toolkitquick (for use with C++ apps which use a QtQuick UI)
- toolkitwidgets (for use with C++ apps which use a QWidgets UI)

You need to clone this repository and then build the project(s) locally before you can incorporate it into your own project.
The projects are desiigned to be build using qmake and QtCreator.

### Shared project resources
All 3 projects make use of a set of shared resources (such as images) and configuration properties so make it easier to build/use tools for different architectures.


#### toolkit_config.pri
The `toolkit_config.pri` allows you to set the output location for toolkit libaries and plugins by setting the `TOOLKIT_INSTALL` variable. It is also where we set the version of the ArcGIS Runtime to use with the toolkit (currently 100.6)

#### /shared
The `/shared` directory contains a set of shared elements for use by the various projects:

- `/shared/cpp` contains C++ code which can be shared between QWidgets and QtQuick tools. The code is branched according to a `WIDGETS_TOOLKIT` #define
- `images` contains a Qt resource files (`.qrc`) containing icons and images for use with tools.
- `qml` contains QtQuick UI files (`.qml`) for use in tools from the QML of C++ QtQuick projects.

### toolkitqml
The `toolkitqml` project can be built to create a static QML plugin library for use with QML only API.

#### building
- in QtCreator open `/toolkitqml/toolkitqml.pro` to open the project.
- locate the `# TOOLS` section of the `.pro` file and comment out any tools you _do not_ wish to include in the library.
- build the project
- run `make install` on windows you can add a custom build step to do this using the `jom` library - e.g. 

`C:\Qt\Tools\QtCreator\bin\jom.exe install`

- the project should output to the `TOOLKIT_INSTALL` location specified in the `toolkit_config.pri` file under a `toolkitqml` parent directory.
- the output should contain a qml plugin under `/esri/arcgisruntime/toolkit`

#### importing the toolkit
A good way to start using the toolkit is to use one of the template apps which get added to QtCreator when you install the ArcGIS Runime SDK for Qt.
- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.6 Quick QML App
- choose settings to match the platform etc. you are building for
- in the .pro` file of your new app, add a line to add the library for your QML plugin - for example:

```
LIBS += -LC:\development\qt_toolkit_prototype\output\toolkitqml\imports\esri\arcgisruntime\toolkit \
        -ltoolkit
```

- in the Run environment settings for the app, add a new environment variable to import the QML module - e.g:

`QML2_IMPORT_PATH = C:/development/qt_toolkit_prototype/output/toolkitqml/imports`

- in `main.cpp` add a line to include the `QtPlugin` library:

`#include <QtPlugin>`

- `main.cpp` (outside of any function definitions) add he macro to import the plugin:

```Q_IMPORT_PLUGIN(ToolkitqmlPlugin)```

#### using a tool from the toolkit
Once you have successfully imported the toolkit, you can access individual tools in your own QML files.
- add an import statement for the toolkit:
`import esri.arcgisruntime.toolkit 1.0`
- declare the tool you wish to use. Generally you will also have to supply the `GeoView` etc. you wish the tool to work with. For example, to add a `NorthArrow`:

```
NorthArrow {
        id: arrow
        width: 32
        height: 32
        anchors {
            right: parent.right
            bottom: view.bottom
        }

        mapView: view
    }
```

### toolkitquick
The `toolkitquick` project can be built to create a static QML plugin library for use with C++ QtQuick version of the API.

#### building
- in QtCreator open `/toolkitquick/toolkitquick.pro` to open the project.
- locate the `# TOOLS` section of the `.pro` file and comment out any tools you _do not_ wish to include in the library.
- build the project
- run `make install`. On windows you can add a custom build step to do this using the `jom` library - e.g. 

`C:\Qt\Tools\QtCreator\bin\jom.exe install`

- the project should output to the `TOOLKIT_INSTALL` location specified in the `toolkit_config.pri` file under a `toolkitquick` parent directory.
- the output should contain a qml plugin under `/esri/arcgisruntime/toolkit`

#### importing the toolkit
A good way to start using the toolkit is to use one of the template apps which get added to QtCreator when you install the ArcGIS Runime SDK for Qt.
- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.6 Qt Quick C++ App
- choose settings to match the platform etc. you are building for
- in the .pro` file of your new app, add a line to add the library for your QML plugin - for example:

```
LIBS += -LC:\development\qt_toolkit_prototype\output\toolkitquick\imports\esri\arcgisruntime\toolkit \
        -ltoolkit
```

- in the Run environment settings for the app, add a new environment variable to import the QML module - e.g:

`QML2_IMPORT_PATH = C:/development/qt_toolkit_prototype/output/toolkitquick/imports`

- in `main.cpp` add a line to include the `QtPlugin` library:

`#include <QtPlugin>`

- `main.cpp` (outside of any function definitions) add he marco to import the plugin:

```Q_IMPORT_PLUGIN(ToolkitquickPlugin)```

#### using a tool from the toolkit
Once you have successfully imported the toolkit, you can access individual tools in your own QML files.
- add an import statement for the toolkit:
`import esri.arcgisruntime.toolkit 1.0`
- declare the tool you wish to use. Generally you will also have to supply the `GeoView` etc. you wish the tool to work with. For example, to add a `NorthArrow`:

```
NorthArrow {
        id: arrow
        width: 32
        height: 32
        anchors {
            right: parent.right
            bottom: view.bottom
        }

        mapView: view
    }
```

### toolkitwidgets
The `toolkitwidgets` project can be built to create a static library for use with C++ QtQuick version of the API.

#### building
- in QtCreator open `/toolkitwidgets/toolkitwidgets.pro` to open the project.
- locate the `# TOOLS` section of the `.pro` file and comment out any tools you _do not_ wish to include in the library.
- build the project
- run `make install`. On windows you can add a custom build step to do this using the `jom` library - e.g. 

`C:\Qt\Tools\QtCreator\bin\jom.exe install`

- the project should output to the `TOOLKIT_INSTALL` location specified in the `toolkit_config.pri` file under a `toolkitwidgets` parent directory.
- the output should contain a `lib` and an `include` directory

#### importing the toolkit
A good way to start using the toolkit is to use one of the template apps which get added to QtCreator when you install the ArcGIS Runtime SDK for Qt.
- In QtCreator choose `File/New file or project/ArcGIS/ArcGIS Runtime 100.6 Qt Widgets App
- choose settings to match the platform etc. you are building for
- in the .pro` file of your new app, add a line to add the library for your QML plugin - for example:

```
LIBS += -LC:\development\qt_toolkit_prototype\output\toolkitwidgets\imports\esri\arcgisruntime\toolkit \
        -ltoolkit
```

- add the include path for the libary headers:

`INCLUDEPATH += C:\development\qt_toolkit_prototype\output\toolkitwidgets\include`

#### using a tool from the toolkit
Once you have successfully imported the toolkit, you can create individual tools in your own widgets files.
- add the include header for the tool you wish to use:

`#include "NorthArrow.h"`

- in your widgets code, create a new instance of the tool you wish to use and add it your widets layout. You will also normally need to pass the `GeoView` which the tool is designed to work with:

```
  auto northArrow = new NorthArrow(this);
  northArrow->setGeometry(0,0,48,48); // set size and position for the tool
  northArrow->setMapView(m_mapView);
  northArrow->show();
```
 





