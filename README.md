ArcGIS Runtime API Toolkit
==========================

Current Release: 100.1

This project contains QML source code for controls and utilities you can use with the [ArcGIS Runtime SDK for Qt](http://developers.arcgis.com/qt). There are QML examples in this repo that demonstrate the use of these controls and how they can be used as resuable building blocks for your QML-based ArcGIS applications.

## Features
- UserCredentials Dialog - a dialog that takes in a username and password. It can be connected with a UserCredentials instance and a Portal instance to sign into Portal for ArcGIS or ArcGIS Online. The dialog is customizable, as the top banner and several labels can be changed.
- AuthenticationView Dialog - a wrapper view that will automatically display the proper authentication view for any of the supported authentication types (OAuth, Token, HTTP Basic, HTTP Digest, SAML, PKI)
- ClientCertificateView Dialog - a dialog for handling PKI authentication
- OAuth2View Dialog - a dialog for handling OAuth authentication
- SslHandshakeView Dialog - a dialog for allowing/blocking SSL Handshake related issues
- UserCredentialsView Dialog - a generic dialog for handling username/password authentication
- NavigationToolbar Control - a toolbar control with 4 buttons: 
    -   Show your location on the map
    -   Zoom in
    -   Zoom out
    -   Home (Back to full extent)
- NorthArrow Control - a compass that points toward true north and the map angle in degrees as the map rotates.
- OverviewMap Control - an overview map control.
- StyleToolbar Control - a way to style your toolbars and buttons they way you want.
- SearchBox Control - allows you to search the map content for addresses and places.
- Callout Control - a view for displaying information at a geographic location on the map.
- PopupView Control - a view for displaying attributes and attachments for features.
- AttributeListView Control - a view for displaying and editing attributes of Features and Graphics.

## Setup instructions 

1. Fork and then clone the repo or download the .zip file.
2. The Toolkit components are designed to be used with the ArcGIS Runtime SDK for Qt. Confirm that your system meets the [minimum requirements](https://developers.arcgis.com/qt/latest/qml/guide/arcgis-runtime-sdk-for-qt-system-requirements.htm) for using the ArcGIS Runtime SDK for Qt. See the below version table to ensure you have the correct version of the Toolkit and ArcGIS Runtime.
3. Add a QML Import Path that points to your local repository's installated location into your project. The process for this is documented in the [Import Statements](http://doc.qt.io/qt-5/qtqml-syntax-imports.html#qml-import-path) Qt documentation. For example, you could do any of the following:
  - Add an environment variable - `QML2_IMPORT_PATH=<path_to_repo>/Import`
  - Add a qmake variable - `QML_IMPORT_PATH+=<path_to_repo>/Import`
  - Add the path to the QML Engine's path list in C++ code - `QQmlEngine::addImportPath("<path_to_repo>/Import");`
4. Import the QML plugin modules in your project. For example:
  - `import Esri.ArcGISRuntime.Toolkit.Controls 2.1`
  - `import Esri.ArcGISRuntime.Toolkit.Dialogs 2.1`

### Run the example application:
1. From Qt Creator, go to <b>File->Open File or Project...</b> on the toolbar, browse for the .qml files from your cloned repo, and open them. 
2. Once these have been added to your Qt Creator, double-click "Example.qml" to open it in the editor. 
3. To run the QML Example app, on the toolbar go to <b>Tools->External->ArcGIS Runtime->ArcGIS Runtime Sample App</b>. The QML Example app with open, and you can use the pulldown to select each individual example program that demonstrates a specific control.
4. Browse the QML code to see how to utilize the control

## Version
Each version of the Toolkit has a minimim version of ArcGIS Runtime that it requires. Older versions of the Toolkit are compatible with newer versions of ArcGIS Runtime, but not vice-versa.

| Toolkit version | Minimum version of ArcGIS Runtime |
|-----------------|-----------------------------------|
| 2.0             | 100.0                             |
| 2.1             | 100.1                             |


## Resources

* [ArcGIS Runtime SDK for Qt](https://developers.arcgis.com/qt/)
* [Qt and QML](http://www.qt.io/)

## Issues

Find a bug or want to request a new feature?  Please let us know by submitting an issue.

## Contributing

Anyone and everyone is welcome to contribute.

## Licensing
Copyright 2014 Esri

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

A copy of the license is available in the repository's [license.txt](license.txt) file.


[](Esri Tags: ArcGIS Runtime SDK Qt QML)
[](Esri Language: Qt)

