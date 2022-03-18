# Changelog

## 100.14
* (ScrollBar) Calcite styling of the ScrollBar, not matching specific component.
* (FloorFilter) Viewpoint selection mode implemented.
* (FloorFilter) Introduction of new FloorFilter tool (C++/Quick, QML/Quick, Widget) 
* (ToolBar) Calcite styling of the ToolBar, similar as a Calcite ActionBar.
* (ToolSeparator) Calcite styling of the ToolSeparator, not maching specific component.
* (ToolButton) Calcite styling of the ToolButton, now matching the Calcite Action component. A second styling is available for flat ToolButton.
* (RadioDelegate) Calcite styling of Radiodelegate component, now matches the Calcite Dropdown component.
* (SpinBox) Calcite styling of the SpinBox component.
* (Callout) Major rewrite of Callout. Moved from Canvas to Shape rendering. Callout is now style compliant.
* Adding unit and functional test suites for uitools.

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
