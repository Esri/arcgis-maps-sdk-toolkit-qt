# ArcGIS Runtime Toolkit - Qt

[![API reference](https://img.shields.io/badge/API_Reference-purple)](https://developers.arcgis.com/qt/latest/toolkit/api-reference/) ![Current release](https://img.shields.io/github/v/release/esri/arcgis-runtime-toolkit-qt?label=current%20release)

## Introduction

This toolkit is an open source resource intended for use with the ArcGIS Runtime
SDK for Qt. It provides ready made components to complement mapping and GIS
applications. Because it is open source you are encouraged to modify these tools
and add your own.

## Integrating with QML

* Clone the Toolkit repo
* In your project's .pro file, include the QML toolkit .pri file - `include(/arcgis-runtime-toolkit-qt/uitools/toolkitqml.pri)`
* Open your main.qml and import the toolkit - `import Esri.ArcGISRuntime.Toolkit 100.10`
* Declare a toolkit component:

```qml
NorthArrow {
    width: 30
    height: width
    geoView: mapView
}
```

## Integrating with Qt Quick C++

* Clone the Toolkit repo
* In your project's .pro file, include the C++ toolkit .pri file - `include(/arcgis-runtime-toolkit-qt/uitools/toolkitcpp.pri)`
* In your project's main.cpp, add `#include "Esri/ArcGISRuntime/Toolkit/register.h"`
* In your project's main.cpp, add the following before the engine is loaded `Esri::ArcGISRuntime::Toolkit::registerComponents();`
* Open your main.qml and import the toolkit - `import Esri.ArcGISRuntime.Toolkit 100.10`
* Declare a toolkit component:

```qml
NorthArrow {
    width: 30
    height: width
    geoView: mapView
}
```

## Integrating with Qt Widgets C++

* Clone the Toolkit repo
* In your project's .pro file, include the Widgets toolkit .pri file - `include(/arcgis-runtime-toolkit-qt/uitools/toolkitwidgets.pri)`
* In your project's QWidget derived class, add `#include "NorthArrow.h"`
* Create the NorthArrow widget and add it to a layout:

```cpp
Esri::ArcGISRuntime::Toolkit::NorthArrow* northArrow = new Esri::ArcGISRuntime::Toolkit::NorthArrow(this);
northArrow->setFixedSize(100, 100);
northArrow->setMapView(m_mapView);

QVBoxLayout* layout = new QVBoxLayout(this);    
layout->addWidget(northArrow);
setLayout(layout);
```

## Resources

* [ArcGIS Runtime SDK for Qt](https://developers.arcgis.com/qt/)
* [System requirements](https://developers.arcgis.com/qt/latest/qml/guide/system-requirements.htm)
* [Qt and QML](http://www.qt.io/)

## Issues

Find a bug or want to request a new feature?  Please let us know by [submitting an issue](https://github.com/Esri/arcgis-runtime-toolkit-qt/issues/new).

## Contributing

Anyone and everyone is welcome to contribute.

## Licensing

Copyright 2019-2020 Esri

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

A copy of the license is available in the repository's [LICENSE](LICENSE) file.

Some files (ios_config.prf) are from the Qt SDK and modified to support the Toolkit.
See https://www.qt.io/terms-conditions/ for license details for those files.
