# Changelog

## 100.15

* (Callout) Deprecated `calloutContent` property in favour of `contentItem` usage.
* Updated `.png` files to be `.svg` instead. Tools no longer look pixilated on high-dpi devices.
* (NorthArrow) Fixed deprecation warning in widgets.
* (CoordinateConversion) Fixed button images not rendering.
* Added a `CONTRIBUTING.md`.
* Added a `.clang-format` file.
* Added vscode/spellchecking for documentation purposes.
* (BookmarksView) Introduction of new Bookmark tool (C++/Quick, QML/Quick, Widget) 

## 100.14
* `toolkitwidgets.pri`, `toolkitqml.pri`, and `toolkitcpp.pri` now include `QT += ...` lines to account for needed dependencies.
* (TimeSlider) Forward and backward buttons are not enabled while playing. Holding buttons, continuously skips forward or backwards. Time labels wrap into new line for small screens and don't overlap behind the buttons.
* (ScrollBar) Calcite styling of the ScrollBar, not matching specific component.
* (FloorFilter) Viewpoint selection mode implemented.
* (FloorFilter) Introduction of new FloorFilter tool (C++/Quick, QML/Quick, Widget) 
* (ToolBar) Calcite styling of the ToolBar, similar as a Calcite ActionBar.
* (ToolSeparator) Calcite styling of the ToolSeparator, not matching specific component.
* (ToolButton) Calcite styling of the ToolButton, now matching the Calcite Action component. A second styling is available for flat ToolButton.
* (RadioDelegate) Calcite styling of RadioDelegate component, now matches the Calcite Dropdown component.
* (SpinBox) Calcite styling of the SpinBox component.
* (Callout) Major rewrite of Callout. Moved from Canvas to Shape rendering. Callout is now style compliant.
* Adding unit and functional test suites for uitools.
* (PopupView) Setting `closeCallback` to `null` now correctly hides the close button as docs advertise.

## 100.13

* (AR) Fixed bug when targeting API 30 on Android related to device permissions.
* (SearchView) Introduction of new SearchView tool (C++/Quick and QML/Quick only).
* (Scalebar) Introduction of new Scalebar tool (C++/Quick only).
* (OverviewMap) Fixed initialization bug wherein the inset would not be valid until first user interaction.
* (BasemapGallery) BasemapGallery is now available as a widget tool.
* (BasemapGallery) Busy indicator added to gallery to show when an item has been clicked but basemap is still loading.
* (BasemapGallery) Basemaps in gallery now appear in a sorted order based on title.
* (BasemapGallery) Gallery items with an incompatible spatial reference to the active scene/map will be disabled.
* (BasemapGallery) Fixed UI issues when a QML theme was applied.
* (TimeSlider) Fixed UI issues when a QML theme was applied.
* (CoordinateConversion) Fixed UI issues when a QML theme was applied.
* (PopupView) Fixed UI issues when a QML theme was applied.
* (PopupView) Now respects label aliases when rendering field names.
* (PopupStackView) Next/Back buttons only visible when there are multiple results. 
* Added the new (WIP) Calcite QML style.
* Added a new Calcite style demo app under `calcite-qml/demo`.
* Added a new tool examples folder under `uitools/examples`.
* Documentation improvements.
* Added a CHANGELOG.md
* Updated README.md with a contributors section.

## 100.12

## 100.11

## 100.10

* Introduction of new toolkit architecture.
