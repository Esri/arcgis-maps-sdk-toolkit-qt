[![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt) [![Qt Quick UI components](https://img.shields.io/badge/Qt%20Qt%20Quick%20UI%20components-ea4d13)](../../toolkitcpp/)

# SearchView - Qt Quick UI/QML Type

The [SearchView - Qt Quick UI/QML Type](https://developers.arcgis.com/qt/toolkit/api-reference/qml-esri-arcgisruntime-toolkit-searchview.html) control enables users to search for one or more locations or features, from a variety of sources, with suggestions. 

## Accessibility

The search field, Clear button, and Search button expose accessible names. Suggestions and results are exposed as named lists with selectable items. The control politely announces the item count after the list settles and announces when no results are found without moving focus.

Use Down from the search field to move focus into the list. Use Up and Down to move between items and Enter to select the current item. Typing or pressing Backspace while the list has focus returns focus to the search field. Escape dismisses an open tooltip or suggestion list.

On iOS, when the search text field has focus, UIKit handles hardware keyboard Tab and Shift+Tab input before Qt receives a key event. As a result, these keys cannot move focus through the control and may insert whitespace into the search text.

Announcement delivery and navigation depend on the platform, keyboard settings, and screen reader. Verify them in your application.

![SearchView image](https://developers.arcgis.com/qt/toolkit/api-reference/images/search.png)