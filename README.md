[![ArcGIS Maps SDK for Qt Toolkit API reference](https://img.shields.io/badge/API_Reference-purple)](https://developers.arcgis.com/qt/latest/toolkit/api-reference/) [![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![Current release](https://img.shields.io/github/v/release/esri/arcgis-maps-sdk-toolkit-qt?label=current%20release)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/releases)

# ArcGIS Maps SDK for Qt Toolkit

This toolkit is an open source resource intended for use with the ArcGIS Maps SDK for Qt. It provides ready made components to complement mapping and GIS applications. Because it is open source you are encouraged to modify these tools and add your own.

Before you can utilize the toolkit, you should first ensure your development machine meets the [system requirements](https://developers.arcgis.com/qt/reference/system-requirements/) and has the ArcGIS Maps SDK for Qt properly [installed and set up](https://developers.arcgis.com/qt/install-and-set-up/). TIP: the toolkit requires the installation of the **Qt WebEngine** which can be found under **Select Components > Extensions** section of the Qt installer.

## Using this toolkit

To use the toolkit, you need to copy this repository onto your development machine.

You can do this by doing either of the following:
- Cloning this repo using Git (using the the URL: https://github.com/Esri/arcgis-maps-sdk-toolkit-qt.git) to your preferred location on your development machine. 
- Downloading the .zip version of the repo and unzipping it to your preferred location on your development machine. Do this by clicking the green "Code" button above and clicking the "Download ZIP" option.

## Toolkit components available

[Qt Quick UI components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/uitools/toolkitcpp) - these are components that compliment your `MapView` or `SceneView` by adding them to your Qt project's .qml files (QML based UI development). They are components like: Authenticator, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for device based apps like phones and tablets. 

[Qt Widgets UI components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/uitools/toolkitwidgets) - these are components that compliment your `MapView` or `SceneView` by adding them to your projects .cpp files. They are components like: Authenticator, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for traditional desktop based apps.

The following table lists the various UI components available depending on how your Qt project was created with either Qt Quick UI or Qt Widgets UI components. 

|          Tool         | [Qt Quick UI components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/uitools/toolkitcpp) |            [Qt Widgets UI components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/uitools/toolkitwidgets)            |
|:---------------------:|:--------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------:|
|  Authenticator                    |    [✔](uitools/toolkitcpp/docs/Authenticator.md)        |                                                          |
|    BasemapGallery                 |      [✔](uitools/toolkitcpp/docs/BasemapGallery.md)     |    [✔](uitools/toolkitwidgets/docs/BasemapGallery.md)    |
|     BookmarksView                 |       [✔](uitools/toolkitcpp/docs/BookmarksView.md)     |    [✔](uitools/toolkitwidgets/docs/BookmarksView.md)     |
|        Callout                    |         [✔](uitools/toolkitcpp/docs/Callout.md)         |                                                          |
| CoordinateConversion              |   [✔](uitools/toolkitcpp/docs/CoordinateConversion.md)  | [✔](uitools/toolkitwidgets/docs/CoordinateConversion.md) |
|      FloorFilter                  |       [✔](uitools/toolkitcpp/docs/FloorFilter.md)       |      [✔](uitools/toolkitwidgets/docs/FloorFilter.md)     |
|      NorthArrow                   |        [✔](uitools/toolkitcpp/docs/NorthArrow.md)       |      [✔](uitools/toolkitwidgets/docs/NorthArrow.md)      |
|      OverviewMap                  |       [✔](uitools/toolkitcpp/docs/OverviewMap.md)       |      [✔](uitools/toolkitwidgets/docs/OverviewMap.md)     |
|       PopupView                   |        [✔](uitools/toolkitcpp/docs/PopupView.md)        |                                                          |
|        Scalebar                   |         [✔](uitools/toolkitcpp/docs/Scalebar.md)        |                                                          |
|      SearchView                   |        [✔](uitools/toolkitcpp/docs/SearchView.md)       |                                                          |
|      TimeSlider                   |        [✔](uitools/toolkitcpp/docs/TimeSlider.md)       |                                                          |
| UtilityNetworkTrace               |  [✔](uitools/toolkitcpp/docs/UtilityNetworkTrace.md)    |                                                          |

## Other Toolkit features available

[Calcite style](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/calcite) - this feature provides visual styles that enable you to create beautiful and consistent experiences using the Calcite web style for Esri on UI controls you have in your app. These styles are QML compatible and are configured by adding them to your .qml files. The styles include options for creating Light and Dark themed UI's in you app. There are over 30 styles available that can be used on Qt controls like: Button, CheckBox, Menu, RadioButton, ToolBar, and so forth. 

## Resources

* [ArcGIS Maps SDK for Qt](https://developers.arcgis.com/qt/)
* [System requirements](https://developers.arcgis.com/qt/latest/qml/guide/system-requirements.htm)
* [Qt](http://www.qt.io/)

## Issues

Find a bug or want to request a new feature?  Please let us know by [submitting an issue](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/issues/new).

# Licensing

Copyright 2019-2022 Esri

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

A copy of the license is available in the repository's [LICENSE](LICENSE) file.
