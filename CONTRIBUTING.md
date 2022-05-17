# Contributing 

Welcome to the contributors' guide on the ArcGIS Runtime toolkit! We hope this document
encourages you to help improve and expand upon our toolkit offering.

For specific resources on:

- How to contribute a new UI tool, see here [here](uitools/CONTRIBUTING.md).
- How to update the Calcite style system, see [here](calcite-qml/CONTRIBUTING.md).

## Environment details

You will need:

- System requirements as laid out [here](https://developers.arcgis.com/qt/reference/system-requirements/)
  for your development platform.
- Latest version of the [ArcGIS Runtime SDK](https://developers.arcgis.com/qt/).

## Important resources

- The [Qt ArcGIS Runtime API Reference](https://developers.arcgis.com/qt/api-reference/). Which includes:
  - [Qt Toolkit API](https://developers.arcgis.com/qt/toolkit/api-reference/).
  - [QML API](https://developers.arcgis.com/qt/qml/api-reference/).
  - [C++ API](https://developers.arcgis.com/qt/cpp/api-reference/).

- The [Qt ArcGIS Runtime Guide](https://developers.arcgis.com/qt/).

- [Qt's board on Esri Community](https://community.esri.com/t5/arcgis-runtime-sdk-for-qt-questions/bd-p/arcgis-runtime-sdk-for-qt-questions).

- [Official Qt documentation](https://doc.qt.io/).

- [DSA App](https://github.com/Esri/dynamic-situational-awareness-qt), an open source project built with the ArcGIS Runtime Toolkit for Qt.

## Submitting a change

There are two active branches within toolkit: `main` and `v.next`.

- **main** is the active branch, and must always be compatible with 
  the [release version of ArcGIS Runtime for Qt](https://developers.arcgis.com/qt/). This is 
  the main branch all our users consume to access toolkit features.

- **v.next** is the "next version" branch which contains upcoming features and is
  compatible with the upcoming version of ArcGIS Runtime. While anyone may contribute to this 
  branch, public contributors outside of Esri will not be able to build this branch as the upcoming
  version of ArcGIS Runtime will be unavailable.

### Bugfixes

If you are contributing a bugfix, and you believe the bug is high priority
in impact, then target the **main** branch in your PR. Otherwise consider targeting **v.next**! 
If unsure, the PR reviewer can help make the right call.

### Feature enhancements

Once you have implemented a new feature, and are ready for a pull-request,  please target the **v.next** branch. 
This helps the toolkit team verify the feature is production ready before we ship!

### CHANGELOG.md

Please always document your update in [CHANGELOG.md](CHANGELOG.md).

### Who should review my pull-request?

If you are a public contributor from outside Esri, ask [ldanzinger](https://github.com/ldanzinger) (product owner), 
for a review! He will review/assign an appropriate developer to look into your contribution.

## Reporting a bug / requesting enhancements

Report all bugfixes/enhancements to [https://github.com/Esri/arcgis-runtime-toolkit-qt/issues](issues). 
Be clear in your reproduction steps and use your best judgement for communicating your problem.

## Testing

We do not enforce that public contributors add to the testing suite, but we encourage it.
If submitting a new feature, please consider adding an example to one of the example apps.

Resources can be found as follows:

- Functional/unit tests can be found in [tests](tests).

- Augmented reality examples can be found in [augmentedreality/Examples](augmentedreality/Examples).

- UI tool examples can be found in [uitools/examples](uitools/examples).

- The Calcite QML style demo can be found in [calcite-qml/demo](calcite-qml/demo).

## Where can I ask for help? 

Often, users ask questions on [Esri Community](https://community.esri.com/t5/arcgis-runtime-sdk-for-qt-questions/bd-p/arcgis-runtime-sdk-for-qt-questions)! The ArcGIS Qt team are very good at answering questions in a timely manner, and 
it is a fantastic repository of past questions.

## Recognition model 

We are happy to take public contributions to the toolkit. At the end of each release we will update our
[contributors section](https://github.com/Esri/arcgis-runtime-toolkit-qt#contributors) of the main `README.md` with 
the accounts of everyone who has made a contribution to the toolkit.

## Code of Conduct

TBD.

## Style Guide / Coding conventions

### QML

For QML code, please follow the official [QML coding conventions](https://doc.qt.io/qt-5/qml-codingconventions.html).

### C++

For C++, please run `clang-format` before submitting your PR. `clang-format` should automatically pick up the `.clang-format`
file found [here](.clang-format).

(Take a look at [this Qt blog post](https://www.qt.io/blog/2019/04/17/clangformat-plugin-qt-creator-4-9) on enabling clang-format to run in Qt creator.)
