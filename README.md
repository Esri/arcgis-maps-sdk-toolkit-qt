ArcGIS Runtime API Toolkit
==========================

Current Release: 100.5

This project contains QML source code for controls and utilities you can use with the [ArcGIS Runtime SDK for Qt](http://developers.arcgis.com/qt). There are QML examples in this repo that demonstrate the use of these controls and how they can be used as resuable building blocks for your QML-based ArcGIS applications.

## Features
- AuthenticationView Dialog - A wrapper view that will automatically display the proper authentication view for any of the supported authentication types (OAuth, Token, HTTP Basic, HTTP Digest, SAML, PKI).
- ClientCertificateView Dialog - A dialog for handling PKI authentication.
- OAuth2View Dialog - A dialog for handling OAuth authentication.
- SslHandshakeView Dialog - A dialog for allowing/blocking SSL Handshake related issues.
- UserCredentialsView Dialog - A generic dialog for handling username/password authentication.
- Callout Control - A view for displaying information at a geographic location on the map.
- PopupView Control - A view for displaying attributes and attachments for features.
- TimeSlider Control - A control for visualizing temporal data by changing the currently visible time extent.
- Compass Control (C++ only) - A compass (aka north arrow) control for indicating which direction is north.
- Coordinate Conversion Control (C++ Only) - A tool for assisting in converting coordinates from one format to another.

## Setup instructions 

1. Fork and then clone the repo or download the .zip file.
2. The Toolkit components are designed to be used with the ArcGIS Runtime SDK for Qt. Confirm that your system meets the [minimum requirements](https://developers.arcgis.com/qt/latest/qml/guide/arcgis-runtime-sdk-for-qt-system-requirements.htm) for using the ArcGIS Runtime SDK for Qt. See the below [version section](#version) to ensure you have the correct version of the Toolkit and ArcGIS Runtime.
3. Add a QML Import Path that points to your local repository's installed location into your project. The process for this is documented in the [Import Statements](http://doc.qt.io/qt-5/qtqml-syntax-imports.html#qml-import-path) Qt documentation. For example, you could do any of the following:
  - Add an environment variable - `QML2_IMPORT_PATH=<path_to_repo>/Import`
  - Add a qmake variable - `QML_IMPORT_PATH+=<path_to_repo>/Import`
  - Add the path to the QML Engine's path list in C++ code - `QQmlEngine::addImportPath("<path_to_repo>/Import");`
4. Import the QML plugin modules in your project. For example:
  - `import Esri.ArcGISRuntime.Toolkit.Controls 100.5`
  - `import Esri.ArcGISRuntime.Toolkit.Dialogs 100.5`

## Alternate Setup instructions (importing the Toolkit as a plugin)

Starting with Qt 5.9.0 on iOS, the Toolkit must be imported as a plugin.

1. Follow the steps above (Setup instructions ) but skip step 3 for adding the Qml import paths to your project.
2. Include [ArcGISRuntimeToolkitPlugin.pri](Plugin/ArcGISRuntimeToolkitPlugin.pri) in your project file.

For example, 
```qmake 
include(/Users/james/ArcGIS_SDKs/Qt100.5/sdk/toolkit/Plugin/ArcGISRuntimeToolkitPlugin.pri)
```

3. The Toolkit will now be compiled into your application directly as a plugin.

## Running the toolkit demo project
The toolkit repo has a demo project located in the Examples folder. This project is not meant to be a template to base your projects off of, but rather to be a quick and easy way to test and demonstrate the capabilities of various toolkit components. You can run the demo project by doing the following steps:

1. Open `Examples.qmlproject` in Qt Creator
2. Go to Projects > Run, and add a new variable called QML2_IMPORT_PATH, and set the value equal to the path of ArcGIS Runtime and the Toolkit. For example, `QML2_IMPORT_PATH=/Users/<username>/ArcGIS_SDKs/Qt100.5/sdk/macOS/x64/qml:/Users/<username>/ArcGIS_SDKs/Qt100.5/sdk/toolkit/Import`
3. Depending on your system, you may additionally need to add the path to the EsriCommonQt library into an environment variable. On Windows, add it to the PATH, on Linux, add it to LD_LIBRARY_PATH, and on macOS, add it to DYLD_LIBRARY_PATH. For example, `PATH=C:\Program Files (x86)\ArcGIS SDKs\Qt100.5\sdk\windows\x64\bin`.
4. The qmlscene application on Windows and Linux require that the path to the Qt libraries be specified in the PATH and LD_LIBRARY_PATH, respectively. For example, `PATH=C:\Qt5.9.2\msvc2015_64\bin`.
5. Run the app and select an example from the drop down.

## Version
The Toolkit is forwards compatible with the ArcGIS Runtime API. It is not backwards compatible. This means that version 100.4 of the Toolkit can be used with version 100.5 of the Runtime API. However, version 100.5 of the Toolkit cannot be used with version 100.4 of the Runtime API, as this version of the Toolkit may require functionality from a newer version of the Runtime API.

## Use a previous version
The `master` branch contains toolkit components for the current, most up-to-date version of ArcGIS Runtime. To access previous versions, you can checkout by a tag or commit, or alternativley you can download the source zip:

- [100.0](https://github.com/Esri/arcgis-runtime-toolkit-qt/releases/tag/1529)
- [100.1](https://github.com/Esri/arcgis-runtime-toolkit-qt/releases/tag/1744)
- [100.2](https://github.com/Esri/arcgis-runtime-toolkit-qt/releases/tag/1912)
- [100.2.1](https://github.com/Esri/arcgis-runtime-toolkit-qt/releases/tag/100.2.1)

## Resources

* [ArcGIS Runtime SDK for Qt](https://developers.arcgis.com/qt/)
* [Qt and QML](http://www.qt.io/)

## Issues

Find a bug or want to request a new feature?  Please let us know by submitting an issue.

## Contributing

Anyone and everyone is welcome to contribute.

## Licensing
Copyright 2018 Esri

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

Some files (ios_config.prf) are from the Qt SDK and modified to support the Toolkit.
See https://www.qt.io/terms-conditions/ for license details for those files.
