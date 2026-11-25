[![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt) [![Qt Quick UI components](https://img.shields.io/badge/Qt%20Qt%20Quick%20UI%20components-ea4d13)](../../toolkitcpp/)

# BookmarksView - Qt Quick UI/QML Type

The [BookmarksView - Qt Quick UI/QML Type](https://developers.arcgis.com/qt/toolkit/api-reference/qml-esri-arcgisruntime-toolkit-bookmarksview.html) displays a collection of viewpoints represented by bookmarks from either Webmap/Webscene or are programmatically defined. When the user selects a bookmark from the provided list, the viewpoint in the geoView is set to the new bookmark's view extent.

## Accessibility

Set `Accessible.name` to provide an application-specific name for the bookmark list. The default name is "Bookmarks." Each bookmark is exposed as a named button with its selected state.

The list is a single tab stop. Use Up, Down, Home, and End to move between bookmarks. Use Enter or Space to activate the current bookmark and move the GeoView to its viewpoint.

Navigation depends on the platform, keyboard settings, and screen reader. Verify it in your application.

![BookmarksView image](https://developers.arcgis.com/qt/toolkit/api-reference/images/bookmarksView.gif)