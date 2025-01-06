[![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt) [![Qt Quick UI components](https://img.shields.io/badge/Qt%20Qt%20Quick%20UI%20components-ea4d13)](../../toolkitcpp/)

# FloorFilter - Qt Quick UI/QML Type

The [FloorFilter - Qt Quick UI/QML Type](https://developers.arcgis.com/qt/toolkit/api-reference/qml-floorfilter.html) is a tool for choosing which floor in a facility you want to view. You can either manually select the site and facility before toggling which floor, or you can use the automatic selection if you just want to auto select the currently visible facility and change floors in that facility. The tool allows for the browsing workflow where sites and facilities are manually selected, automatic facility selection based on the GeoView's current viewpoint. This mode is useful when you are exploring the GeoView but do not know the name of the site or facility. For the same reason, it is possible to list all the facilities available in the `FloorManager` using the button on the siteView.

The user interface is driven from the `FloorAware` data that is available in the GeoModel's `FloorManager`.

2D maps and 3D scenes are supported.

![FloorFilter image](../images/FloorFilter.gif)