# UtilityNetworkTrace

The user interface for the UtilityNetworkTrace tool. The UI is only displayed if there is at least one Utility Network loaded in the data model. When there are multiple Utility Networks present in the map, the first one in the list is displayed.

The UI includes a TabBar with one tab for configuring a new trace and another Tab for displaying results.

The new trace Tab includes a custom UtilityNetworkTraceStartingPointsModel for displaying the different properties of starting points in the UI.

The result Tab itself contains a TabBar of the results made up of custom Tabs defined in UtilityNetworkFunctionTraceResultsModel.

Generic styles, displayed in this Qml UI, can be overwritten using the Calcite style.

NEED IMAGE FOR UtilityNetworkTrace

Details on how to use the control can be found here:
- [QML Type/UI Controls/QtQuick](https://developers.arcgis.com/qt/toolkit/api-reference/qml-utilitynetworktrace.html)
- C++ Classes/Qt Widgets - NONE