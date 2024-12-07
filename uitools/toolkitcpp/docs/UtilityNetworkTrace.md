[![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt) [![Qt Quick UI components](https://img.shields.io/badge/Qt%20Qt%20Quick%20UI%20components-ea4d13)](../../toolkitcpp/)

# UtilityNetworkTrace - Qt Quick UI control/QML Type

The [UtilityNetworkTrace - Qt Quick UI control/QML Type](https://developers.arcgis.com/qt/toolkit/api-reference/qml-utilitynetworktrace.html) tool UI is only displayed if there is at least one Utility Network loaded in the data model. When there are multiple Utility Networks present in the map, the first one in the list is displayed.

The UI includes a TabBar with one tab for configuring a new trace and another Tab for displaying results.

The new trace Tab includes a custom UtilityNetworkTraceStartingPointsModel for displaying the different properties of starting points in the UI.

The result Tab itself contains a TabBar of the results made up of custom Tabs defined in UtilityNetworkFunctionTraceResultsModel.

Generic styles, displayed in this Qml UI, can be overwritten using the Calcite style.

NEED IMAGE FOR UtilityNetworkTrace