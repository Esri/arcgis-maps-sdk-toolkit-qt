ArcGIS Runtime API Toolkit
==========================

Current Release: 100.1

This project contains QML source code for controls and utilities you can use with the [ArcGIS Runtime SDK for Qt](http://developers.arcgis.com/qt). There are QML examples in this repo that demonstrate the use of these controls and how they can be used as resuable building blocks for your QML-based ArcGIS applications.

## Features
- UserCredentials Dialog - A dialog that takes in a username and password. It can be connected with a UserCredentials instance and a Portal instance to sign into Portal for ArcGIS or ArcGIS Online. The dialog is customizable, as the top banner and several labels can be changed.
- AuthenticationView Dialog - A wrapper view that will automatically display the proper authentication view for any of the supported authentication types (OAuth, Token, HTTP Basic, HTTP Digest, SAML, PKI).
- ClientCertificateView Dialog - A dialog for handling PKI authentication.
- OAuth2View Dialog - A dialog for handling OAuth authentication.
- SslHandshakeView Dialog - A dialog for allowing/blocking SSL Handshake related issues.
- UserCredentialsView Dialog - A generic dialog for handling username/password authentication.
- Callout Control - A view for displaying information at a geographic location on the map.
- PopupView Control - A view for displaying attributes and attachments for features.

## Setup instructions 

1. Fork and then clone the repo or download the .zip file.
2. The Toolkit components are designed to be used with the ArcGIS Runtime SDK for Qt. Confirm that your system meets the [minimum requirements](https://developers.arcgis.com/qt/latest/qml/guide/arcgis-runtime-sdk-for-qt-system-requirements.htm) for using the ArcGIS Runtime SDK for Qt. See the below [version section](#version) to ensure you have the correct version of the Toolkit and ArcGIS Runtime.
3. Add a QML Import Path that points to your local repository's installed location into your project. The process for this is documented in the [Import Statements](http://doc.qt.io/qt-5/qtqml-syntax-imports.html#qml-import-path) Qt documentation. For example, you could do any of the following:
  - Add an environment variable - `QML2_IMPORT_PATH=<path_to_repo>/Import`
  - Add a qmake variable - `QML_IMPORT_PATH+=<path_to_repo>/Import`
  - Add the path to the QML Engine's path list in C++ code - `QQmlEngine::addImportPath("<path_to_repo>/Import");`
4. Import the QML plugin modules in your project. For example:
  - `import Esri.ArcGISRuntime.Toolkit.Controls 100.1`
  - `import Esri.ArcGISRuntime.Toolkit.Dialogs 100.1`

## Running the example application

1. Open `Examples.qmlproject` in Qt Creator
2. Go to Projects > Run, and add a new variable called QML2_IMPORT_PATH, and set the value equal to the path of ArcGIS Runtime and the Toolkit. For example, `/Users/<username>/ArcGIS_SDKs/Qt100.1/sdk/macOS/x64/qml:/Users/<username>/ArcGIS_SDKs/Qt100.1/sdk/toolkit/Import`
3. Depending on your system, you may additionally need to add the path to the EsriCommonQt library into an environment variable. On Windows, add it to the PATH, on Linux, add it to LD_LIBRARY_PATH, and on macOS, add it to DYLD_LIBRARY_PATH.
4. Run the app and select an example from the drop down.

## Version
The Toolkit is forwards compatible with the ArcGIS Runtime API. It is not backwards compatible. This means that version 100.0 of the Toolkit can be used with version 100.1 of the Runtime API. However, version 100.1 of the Toolkit cannot be used with version 100.0 of the Runtime API, as this version of the Toolkit may require functionality from a newer version of the Runtime API.


## Resources

* [ArcGIS Runtime SDK for Qt](https://developers.arcgis.com/qt/)
* [Qt and QML](http://www.qt.io/)

## Issues

Find a bug or want to request a new feature?  Please let us know by submitting an issue.

## Contributing

Anyone and everyone is welcome to contribute.

## Licensing
Copyright 2017 Esri

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

