# ArcGIS Maps SDK for Qt Toolkit

[![ArcGIS Maps SDK for Qt Toolkit API reference](https://img.shields.io/badge/API_Reference-purple)](https://developers.arcgis.com/qt/latest/toolkit/api-reference/) [![Current release](https://img.shields.io/github/v/release/esri/arcgis-maps-sdk-toolkit-qt?label=current%20release)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/releases) [![ArcGIS Maps SDk for Qt home](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-8A2BE2)(https://developers.arcgis.com/qt/)]

## Introduction

This toolkit is an open source resource intended for use with the ArcGIS Maps SDK for Qt. It provides ready made components to complement mapping and GIS applications. Because it is open source you are encouraged to modify these tools and add your own.

## Using this toolkit

To use the toolkit, you need to copy this repository onto your development machine.

You can do this by doing either of the following:
- Cloning the this repo using Git (using the the URL: https://github.com/Esri/arcgis-maps-sdk-toolkit-qt.git) to your preferred location on your development machine.  Do this by clicking the green "Code" button above and clicking the "Open with GitHub Desktop" option.
- Downloading the .zip version of the repo and unzipping it to your preferred location on your development machine. Do this by clicking the green "Code" button above and clicking the "Download ZIP" option.

## Toolkit component options available

Currently, the toolkit contains:
- [Qt Quick UI components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/uitools/toolkitcpp) - these are controls that compliment your MapView or SceneView by adding them to your Qt project's .qml files (QML based UI development). They are components like: AuthenticationView, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for device based apps like phones and tablets. 
- [Qt Widgets UI components](need URL) - these are controls that compliment your MapView or SceneView by adding them to your projects .cpp files. They are components like: AuthenticationView, NorthArrow, OverviewMap, TimeSlider, and so forth. Using these components are very beneficial for traditional desktop based apps.  You can learn more about the benefits of building apps with the [Qt Widgets UI controls](https://doc.qt.io/qt-6/qtwidgets-index.html). 
- [Calcite style components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/calcite) - these are components that enable you to create beautiful and consistent experiences using the Calcite web style for Esri. These components are QML compatible and are configured by adding them to your .qml files. These components include options for creating Light and Dark themed UI's in you app. There are over 30 controls available like: Button, CheckBox, Menu, RadioButton, ToolBar, and so forth. You can learn more about [Calcite web project](https://esri.github.io/calcite-web/) from the Esri sponsored GitHub repo. 
- [Augmented Reality toolkit components](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/tree/main/augmented_reality) - they provide support for ARKit for iOS apps and ARCore for Android apps.

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

Some files (ios_config.prf) are from the Qt SDK and modified to support the Toolkit. See https://www.qt.io/terms-conditions/ for license details for those files.