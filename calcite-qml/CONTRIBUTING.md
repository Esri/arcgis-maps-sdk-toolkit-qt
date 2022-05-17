# Contributing to Calcite QML.

This `CONTRIBUTING.md` file covers how to specifically contribute to the Qt Toolkit Calcite style. 
For general contributing guidelines see [here](/CONTRIBUTING.md).

## Important resources

- The Qt guide for [customizing Qt Quick controls](https://doc.qt.io/qt-5/qtquickcontrols2-customize.html).

- [Calcite UI icons](https://esri.github.io/calcite-ui-icons/).

- [Calcite components](https://developers.arcgis.com/calcite-design-system/components/).

- The [Calcite Web](https://esri.github.io/calcite-web/documentation/components) style guide.

- Calcite [colors and themes](https://developers.arcgis.com/calcite-design-system/foundations/colors/).

- [Calcite design system](https://developers.arcgis.com/calcite-design-system/).

## Design philosophy 

1. We are writing a custom QML style. Wherever there is a direct conflict between the QML 
and Calcite styling systems, QML should be favored.

2. Coherence is key. Favor designs which look cohesive 
   to the rest of QML Calcite over components which mirror Calcite Web exactly.

3. Avoid many unique properties and/or behaviors on your control. A user should be free to swap out
   [Material](https://doc.qt.io/qt-5/qtquickcontrols2-material.html) or [Universal](https://doc.qt.io/qt-5/qtquickcontrols2-universal.html) for Calcite without major changes in layout or behavior. (Think [Liskov Substitution](https://en.wikipedia.org/wiki/Liskov_substitution_principle)!).

4. Be flexible! Use your eye for design and UX to draw out the best of Calcite in QML.

## Styling system 

Let us quickly go over the typical workflow where a styled component is imported in QML.

1. The developer, in their application, chooses to import a control, e.g. `Button`.
2. QML checks the currently applied style and imports the specific `Button.qml` from within that style folder.
3. The specific `Button.qml` provides the look-and-feel of the button. This inherits from `T.Button.qml`, a [Qt Quick template](https://doc.qt.io/qt-5/qtquicktemplates2-index.html) which handles the business logic for the `Button` type.

In this module, We are primarily interested in step 3. We provide our own custom `Button.qml`, `Slider.qml`, etc. which lay the UX on top of `T.Button.qml` and `T.Slider.qml` respectively.

All UI elements which inherit from [Control](https://doc.qt.io/qt-5/qml-qtquick-controls2-control.html) may
be styled. As more controls are added to [this list](https://doc.qt.io/qt-5/qtquick-controls2-qmlmodule.html), we 
should be proactive in providing a Calcite equivalent in a timely manner. Your contributions are welcome here!

## Contributing guidelines

### Adding a new Control

_Please target branch `v.next` in all pull-requests which add a new feature._

1. When implementing a new control, look at the `Material` style source code as a starting point for implementation specifics. This is a good starting point for how the Qt company style their own components and can be adapted for Calcite. At the time of writing this folder can be found in your installed Qt directory at: `$QT_HOME/qml/QtQuick/Controls.2/Material`.

2. Place your new style file in the [Calcite](Calcite) folder, and update both [Calcite/qmldir](Calcite/qmldir) and 
[Calcite/calcite.qrc](Calcite/calcite.qrc) to reference this new type.

3. It is heavily encouraged that upon adding a new component that you also update the [demo](demo) application.

4. Finally, please update [CHANGELOG.md](/CHANGELOG.md) and the [available tools](README.md#available-tools) section of the Calcite README.md to reflect this addition.

### Bugfixes

See the main [CONTRIBUTING.md guidelines](/Contributing.md#bugfixes) on bugfixes.