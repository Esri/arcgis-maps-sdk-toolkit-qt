# Contributing to UI tools.

This `CONTRIBUTING.md` file covers how to specifically contribute a new UI tool to the toolkit. For general contributing
guidelines see [here](/CONTRIBUTING.md).

## Important resources

- The [Building a better toolkit](https://community.esri.com/t5/arcgis-runtime-sdks-blog/building-a-better-toolkit/ba-p/888826) Esri community blog.

- The [Qt ArcGIS Runtime API Reference](https://developers.arcgis.com/qt/api-reference/). Which includes:
  - [Qt Toolkit API](https://developers.arcgis.com/qt/toolkit/api-reference/).
  - [QML API](https://developers.arcgis.com/qt/qml/api-reference/).
  - [C++ API](https://developers.arcgis.com/qt/cpp/api-reference/).

- The [Qt ArcGIS Runtime Guide](https://developers.arcgis.com/qt/).

## UI Tools Architecture

ArcGisRuntime for Qt provides 3 different GUI workflows.

1. **QtQuick + QML (a.k.a QML API)** The QML API is a 100% QML API. We see it used very effectively in [AppStudio](https://www.esri.com/en-us/arcgis/products/arcgis-appstudio/overview). The overall philosophy of this API is that the user should touch no or as little C++ code as possible.

1. 