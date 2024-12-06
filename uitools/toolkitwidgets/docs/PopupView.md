# PopupView

A view for displaying and editing information about a feature. A PopupView can be used to display information for any type that implements the PopupSource interface. For example, FeatureLayer implements PopupSource. This means that it has a PopupDefinition, which defines how the Popup should look for any features in that layer. An example workflow for displaying a PopupView for a feature in a FeatureLayer would be:

- Declare a PopupView and anchor it to a desired location.
- Perform an identify operation on a GeoView and select a Feature from the identify result.
- Create a Popup from the Feature.
- Optionally obtain the Popup's PopupDefinition and set the title, whether to show attachments, and so on.
- Create a PopupManager from the Popup.
- Assign the PopupView's popupManager property the PopupManager created in the previous step.

The PopupView is a QML Item that can be anchored, given to a dialog, or positioned using XY screen coordinates. Transform, Transition, and other QML animation types can be used to animate the showing and dissmisal of the view. For more information, please see the Popup and PopupManager documentation.

NOTE: Each time a change is made to the Popup, PopupDefinition, PopupManager, or any of their properties, the PopupManager must be re-set to the PopupView.

![PopupView image](https://developers.arcgis.com/qt/toolkit/api-reference/images/popupview.png)

Details on how to use the control can be found here:
- [QML Type/UI Controls/QtQuick](https://developers.arcgis.com/qt/toolkit/api-reference/qml-popupview.html)
- C++ Classes/Qt Widgets - NONE
