
# Augmented Reality (AR) toolkit for ArcGIS Runtime SDK for Qt

The Augmented Reality toolkit provides support for ARKit for iOS and Android.
This documentation describes the installation and uses of the AR toolkit.

## Overview

Augmented reality experiences are designed to "augment" the physical world with virtual content that
respects real world scale, position, and orientation of a device. In the case of Runtime, a SceneView
displays 3D geographic data as virtual content on top of a camera feed which represents the real, physical
world.

The Augmented Reality (AR) toolkit component allows quick and easy integration of AR into your application
for a wide variety of scenarios. The toolkit recognizes the following common patterns for AR:

- **Flyover**: Flyover AR lets you explore a scene using your device as a window into the virtual world.
A typical flyover AR scenario begins with the scene’s virtual camera positioned over an area of interest.
You can walk around and reorient the device to focus on specific content in the scene.
- **Tabletop**: Scene content is anchored to a physical surface, as if it were a 3D-printed model.
- **Real-scale**: Scene content is rendered exactly where it would be in the physical world. A camera feed
is shown and GIS content is rendered on top of that feed. This is used in scenarios ranging from viewing
hidden infrastructure to displaying waypoints for navigation.

The AR toolkit component is comprised of one class: `ArcGISArView`. This is a subclass of QQuickItem, which
contains the functionality needed to display an AR experience in your application. `ArcGISArView` uses `ARKit` (iOS) or
`ARCore` (Android) augmented reality framework to display the live camera feed and handle real world tracking
and synchronization with the Runtime SDK's `SceneView`. The `ArcGISArView` is responsible for starting and
managing an `ARKit` or `ARCore` session. `ArcGISArView` uses a `LocationDataSource` for getting an initial GPS location
and when continuous GPS tracking is required. The `CalibrationView` can be used to change manually the location and
heading of the scene, to ensure accuracy between the device live video and the scene.

For details about using `ARKit`, please refer to [Apple's documentation](https://developer.apple.com/augmented-reality).
For `ARCore`, please refer to [Google's documentation](https://developers.google.com/ar/).

The minimal version of the ArcGIS Runtime SDK for Qt is 100.6.

### Features of the AR component

- Allows display of the live camera feed
- Manages `ARKit` or `ARCore` session lifecycle
- Tracks user location and device orientation through a combination of `ARKit`or `ARCore` and the device GPS
- Provides access to a `SceneView` to display your GIS 3D data over the live camera feed
- `screenToLocation` method to convert a screen point to a real-world coordinate
- Easy access to all `ARKit` or `ARCore` and `LocationDataSource` delegate methods

## Creating an new AR application using ArcGIS Runtime AR toolkit

### Creating a new C++ application with AR support

An example of C++ application with AR support can be found in the folder `Examples\AR\CppArExemple` in
the toolkit repo.

In the following explaination, `<ProjectName>` refere to the project name used to create the Qt's project.

1. Install the ArcGIS Runtime SDK for Qt.

2. Download the sources of the [ArcGIS Runtime API Toolkit](https://github.com/Esri/arcgis-runtime-toolkit-qt).

3. In Qt Create, create a new project and select `ArcGIS Runtime 100.6 Qt Quick C++ app`.
Select the option "3D project" in the "Details" step.

4. In the created project, add these lignes in the `.pro` file. These lines can be anywhere.
Set the correct path to the ArcGIS Toolkit source folder.
```
AR_TOOLKIT_SOURCE_PATH = # must be set to the path to toolkit sources
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/ArCppApi.pri)
```

5. In the `main.cpp` file.

5a. Set the environment variable `QSG_RENDER_LOOP` to `basic` in the begining of the `main` function.
See [Note for performance issues] for details.
```
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```

5b. Include the header `ArcGISArView.h`.
```
#include "ArcGISArView.h"
```

5c. Call the `ArcGISArView::qmlRegisterTypes()` before the creation of the QML engine, to register the
`ArcGISArView` and `LocationDataSource` C++ classes in QML engine. This is necessary to create a object
of these types in QML.
```
Esri::ArcGISRuntime::Toolkit::ArcGISArView::qmlRegisterTypes();
```

6. In the `<ProjectName>.h` file, add a new property to access to AR view:
```
#include "ArcGISArView.h"
...

class <ProjectName> : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView READ arcGISArView WRITE setArcGISArView
             NOTIFY arcGISArViewChanged)
  ...

signals:
  void arcGISArViewChanged();
  ...

private:
  Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView() const;
  void setArcGISArView(Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView);

  Esri::ArcGISRuntime::Toolkit::ArcGISArView* m_arcGISArView = nullptr;
  ...
};
```

7. In the `<ProjectName>.cpp` file, implemente the setter and getter for `arcGISArView` proprety.
In the setter, set the properties `originCamera`, `translationFactor` and `setLocationDataSource`.
An example of settings for full scall mode is given in the following code.
```
using namespace Esri::ArcGISRuntime::Toolkit;
...

ArcGISArView* <ProjectName>::arcGISArView() const
{
  return m_arcGISArView;
}

void <ProjectName>::setArcGISArView(ArcGISArView* arcGISArView)
{
  if (!arcGISArView || arcGISArView == m_arcGISArView)
    return;

  m_arcGISArView = arcGISArView;

  // full scale mode
  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);
  m_arcGISArView->setLocationDataSource(new LocationDataSource(this));

  emit arcGISArViewChanged();
}
```

An example of parameters for tabletop AR app is given in the following code:
```
  m_arcGISArView->setOriginCamera(Camera(latitude, longitude, altitude, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(1000.0);
  m_arcGISArView->setLocationDataSource(nullptr);
```

In the constructeur of <ProjectName>, adapte the creation of the scene.
The default scene is suitable for full scale mode.

8. In the `<ProjectName>Form.qml` file.

8a. Import the AR toolkit:
```
import Esri.ArcGISArToolkit 1.0
```

8b. Create an AR view in the back of the `SceneView` (then before the declaration of `SceneView` in the QML file):
```
ArcGISArView {
    id: arcGISArView
    anchors.fill: parent
    sceneView: view
    tracking: true
}
```

8c. In the declaration of `<ProjectName>` component, bind the AR view to the `arcGISArView` property:
```
<ProjectName> {
    id: model
    arcGISArView: arcGISArView
    ...
```

### Creating a new QML application with AR support

An example of QML application with AR support can be found in the folder `Examples\AR\QmlArExemple` in
the toolkit repo.

1. Install the ArcGIS Runtime SDK for Qt.

2. Download the sources of the [ArcGIS Runtime API Toolkit](https://github.com/Esri/arcgis-runtime-toolkit-qt).

3. In Qt Create, create a new project and select `ArcGIS Runtime 100.6 Qt Quick QML app`.
Select the option "3D project" in the "Details" step.

4. In the created project, add these lignes in the `.pro` file. These lines can be anywhere.
Set the correct path to the ArcGIS Toolkit source folder.
```
AR_TOOLKIT_SOURCE_PATH = # must be set to the path to toolkit sources
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/ArQmlApi.pri)
```

5. In the `main.cpp` file.

5a. Set the environment variable `QSG_RENDER_LOOP` to `basic` in the begining of the `main` function.
See [Note for performance issues] for details.
```
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```

5b. Include the header `QmlArcGISArView.h`.
```
#include "QmlArcGISArView.h"
```

5c. Call the `QmlArcGISArView::qmlRegisterTypes()` before the creation of the QML engine, to register the
`ArcGISArView` and `LocationDataSource` QML types in QML engine. This is necessary to create a object
of these types in QML.
```
Esri::ArcGISRuntime::Toolkit::QmlArcGISArView::qmlRegisterTypes();
```

6. In the `main.qml` file.

6a. Import the AR toolkit:
```
import Esri.ArcGISArToolkit 1.0
```

6b. Create an AR view in the back of the `SceneView` (then before the declaration of `SceneView` in the QML file).
The following code show an example of initialization for the properties `originCamera`, `translationFactor` and
`locationDataSource`used for full scale mode.
```
ArcGISArView {
    id: arcGISArView
    anchors.fill: parent
    tracking: true
    sceneView: sceneView
    originCamera: Camera {}
    locationDataSource: LocationDataSource {}
    translationFactor: 1.0
}
```

The following code show an example for tabletop mode.
```
originCamera: Camera {
    location: Point {
        y: latitude
        x: longitude
        z: altitude
    }
    heading: 0.0
    pitch: 90.0
    roll: 0.0
}
locationDataSource: null
translationFactor: 1000.0
```

## AR frameworks configuration

All the files necessary to build an ArcGIS application with AR support are provided in the AR toolkit.
To deploy the application on Android and iOS devices, extra steps are needed to give the permission
to the application.

### Configuration for iOS devices

1. Open the `Info.plist` file and add the following key at the end of the file, just before `</dict>`,
to give the access to the camera to the `ARKit`.
```
<key>NSCameraUsageDescription</key>
<string>Camera access is needed for AR testing</string>
```

See [NSCameraUsageDescription Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/nscamerausagedescription?language=objc) for details.

2. The project is ready to be build and run in an iOS device. You need to verify the compatibility of your device
with the `ARKit`.

### Configuration for Android devices

For details about `ARCore` configuration, see [ARCore documentation](https://developers.google.com/ar/develop/c/enable-arcore).

1. In Qt Creator.

1a. Open the `Projects` mode in the left toolbar and select an Android kit.

1b. In the `Build Settings` then `Build Android APK`, clic on `Create template` button.
An `AndroidManifest.xml` and `gradle` files are added to the project.

2. Open the `AndroidManifest.xml` file using the plain text editor (right clic on the filename and select
`Open With` then `Plain Text Editor`).

2a. Under the line `<!-- Application arguments -->`, add the following line to enable the ARCore support.
```
<meta-data android:name="com.google.ar.core" android:value="required"/>
```

The value can be `optional` or `required`. An AR optional application can be installed and run on devices
that don't support `ARCore` and the `Google Play Services for AR` is not automatically installed
with the app. An AR required application is available only on devices that support `ARCore` and the
`Google Play Services for AR` is automatically installed.

For details, see [Enable ARCore documentation](https://developers.google.com/ar/develop/java/enable-arcore#manifest).

2b. Under the line `<!-- %%INSERT_FEATURES -->`, add the following line to give the access to
the camera to ARCore.
```
<uses-feature android:name="android.hardware.camera.ar" android:required="true"/>
```

2c. Set the minimal SDK version `android:minSdkVersion` to 24 for AR required application and 14 for
AR optional application.
```
<uses-sdk android:minSdkVersion="24" android:targetSdkVersion="28"/>
```

3. Open the `build.gradle` file. In `dependencies`, add the ARCore dependency:
```
dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar', '*.aar'])
    implementation 'com.google.ar:core:1.11.0'
}
```

4. The project is ready to be built and run on an Android device. You need to verify the compatibility of your device
with the ARKit.

## Note for performance issues

There are some conflicts between the AR frameworks and the Qt's rendering thread.
These lines of code enable the non-threaded render loop mode in Qt.

See `SceneView::renderFrame` documentation and
[Qt's documentation](https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows)
for more information.

```
#if defined(Q_OS_IOS) && defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```
