# Augmented Reality toolkit components

The Augmented Reality (AR) toolkit components provide support for ARKit for iOS
apps and ARCore for Android apps written using ArcGIS Maps SDK for Qt. This
documentation describes the installation and uses of the AR toolkit components.

## Overview

Augmented reality experiences augment the physical world with
virtual content that respects real world scale, position, and
orientation of a device. A `SceneView` object displays 3D geographic AR
data as virtual content on top of a camera feed of the real, physical world.

The AR toolkit components facilitates integrating AR into your
apps for a wide variety of scenarios. The toolkit recognizes the
following common patterns for AR:

- **Flyover**: Flyover AR lets you explore a scene using your device as a
window into the virtual world. A typical flyover AR scenario begins with the
scene’s virtual camera positioned over an area of interest. You can walk around
and reorient the device to focus on specific content in the scene.

- **Tabletop**: Scene content is anchored to a physical surface, appearing as
if the scene were a 3D physical model.

- **World-scale**: Scene content is rendered exactly where it would be in the
physical world as viewed through a device's camera feed. The camera feed
provides physical context to the data content, with data content rendered on
top of that feed. This pattern is used in scenarios ranging from viewing hidden
infrastructure, such as the recorded position of utility lines or pipes below a
street, to displaying waypoints for navigation.

The AR toolkit components include the `ArcGISArView` class, a
subclass of `QQuickItem`, which contains the functionality your app needs to display an AR experience. `ArcGISArView` uses ARKit (iOS) or
ARCore (Android) augmented reality framework to display the live camera feed
and handle real world tracking and synchronization with the
`SceneView` class. The `ArcGISArView` class is responsible for starting and managing an
ARKit or ARCore session. `ArcGISArView` also uses a `LocationDataSource` class
for getting an initial GPS location when GPS tracking is required.
Use the `CalibrationView` class to manually change the location and
heading of the scene to ensure accuracy between the device live video and the scene.

### Features of the AR toolkit component

- Allows display of the live camera feed
- Manages ARKit or ARCore session lifecycle
- Tracks user location and device orientation through a combination of
  ARKit or ARCore and the device's GPS and other sensors
- Provides access to a `SceneView` to display your GIS 3D data over the live
  camera feed
- Provides the `screenToLocation` method to convert a screen point to a
  real-world coordinate
- Eases access to ARKit, ARCore, and `LocationDataSource` delegate
  methods

### System Requirements

iOS 11 is the minimum requirement. Devices must support ARKit. For details about using ARKit, please refer to [Apple's
documentation](https://developer.apple.com/augmented-reality).

Android 7.0 64-bit (armv8) is the minimum requirement. 32-bit (armv7) is not supported. Devices must support ARCore. For details about using ARCore, please refer to [Google's documentation](https://developers.google.com/ar/).

The latest ArcGIS Maps SDK for Qt is required to use this toolkit component.

The system requirements for the ArcGIS Maps SDK for Qt take precedence over the minimum versions mentioned here.

OpenGL graphics API is required. Metal is not supported. See the [limitations](#Limitations) section below for more details.

## Creating a new AR app using ArcGIS Maps AR toolkit

### Creating a new C++ app

An example C++ app with AR support can be found in the folder
`Examples\ArExample` in the toolkit repo.

Note: In the following explanation, `<ProjectName>` refers to the project name
used to create the Qt project.

1. Install the ArcGIS Maps SDK for Qt. See
[ArcGIS Maps SDK for Qt](https://developers.arcgis.com/qt/install-and-set-up)
for details.

2. Download from GitHub the source files of the
[ArcGIS Maps SDK for Qt Toolkit](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt).

3. In Qt Creator, create a new project and select the app template named
"ArcGIS Maps 200.5.0 Qt Quick C++ app". Select the option "3D project" in the
"Details" dialog.

4. In the newly created project, add the following lines anywhere in the app's project (`.pro`)
file, making sure to use the correct path to the ArcGIS Toolkit source folder
on your development machine.

```qmake
ARCGIS_TOOLKIT_PATH = # must be set to the root path of the toolkit folder
include($$ARCGIS_TOOLKIT_PATH/augmented_reality/ArApi/ArApi.pri)
```

5. In the `main.cpp` file, make the following changes:

  - Set the environment variable `QSG_RENDER_LOOP` to `basic` in the beginning of the `main` function. See [Note for performance issues] for details.

```cpp
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```

  - Include the header `ArcGISArView.h`.

```cpp
#include "ArcGISArView.h"
```

  -  Call the `ArcGISArView::qmlRegisterTypes()` before the creation of the QML
  engine, to register with the QML engine the`ArcGISArView` and
  `LocationDataSource` C++ classes . This is necessary to allow you to create
  objects of these types in QML.

```cpp
Esri::ArcGISRuntime::Toolkit::ArcGISArView::qmlRegisterTypes();
```

6. In the `<ProjectName>.h` file, add a new property to access the AR view:

```cpp
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

7. In the `<ProjectName>.cpp` file, implement the setter and getter for the
`arcGISArView` property. In the setter, set the properties `originCamera`,
`translationFactor`, and `setLocationDataSource`.
An example of settings for world-scale mode is given in the following code.

```cpp
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

```cpp
  m_arcGISArView->setOriginCamera(Camera(latitude, longitude, altitude, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(1000.0);
  m_arcGISArView->setLocationDataSource(nullptr);
```

In the constructor of the app's root class, adapt the creation of the scene.
When creating the [Scene](https://developers.arcgis.com/qt/latest/cpp/api-reference/esri-arcgisruntime-scene.html),
add some data that will visualize well in AR, like an
[IntegratedMeshLayer](https://developers.arcgis.com/qt/latest/cpp/api-reference/esri-arcgisruntime-integratedmeshlayer.html), for example.
The default Scene is suitable for full scale mode.

8. In the `<ProjectName>Form.qml` file,  make the following changes:

  - Import the AR toolkit:

```js
import Esri.ArcGISArToolkit 1.0
```

  - Create an AR view in the back of the `SceneView` (then before the declaration of `SceneView` in the QML file):

```js
ArcGISArView {
    id: arcGISArView
    anchors.fill: parent
    sceneView: view
    tracking: true
}
```

  - In the declaration of `<ProjectName>` component, bind the AR view to the `arcGISArView` property:

```js
<ProjectName> {
    id: model
    arcGISArView: arcGISArView
    ...
}
```

## AR frameworks configuration

All the files necessary to build an ArcGIS app with AR support are
provided in the AR toolkit. To deploy the app on Android and iOS
devices, extra steps are needed to give necessary permissions to the
app.

### Configuration for iOS devices

1. In the `Info.plist` file, add the following key at the end of the file,
just before `</dict>`. This gives ARKit access to the device's camera.

```xml
<key>NSCameraUsageDescription</key>
<string>Camera access is needed for AR testing</string>
```

See [Apple's documentation for NSCameraUsageDescription](https://developer.apple.com/documentation/bundleresources/information_property_list/nscamerausagedescription?language=objc) for details.

2. The project is now ready to be build and run in a compatible iOS device.
ARKit requires an iOS or iPadOS device with iOS 11 and an A9 processor or later.
The list of compatible devices can be found in the end of the
[Augmented Reality page](https://www.apple.com/lae/ios/augmented-reality/).

### Configuration for Android devices

For details about `ARCore` configuration, see [Google's ARCore documentation](https://developers.google.com/ar/develop/c/enable-arcore).

1. In Qt Creator, make these changes to the project:

  - Open the "Projects" mode (in the left toolbar) and select an Android kit.

  - In the "Build Settings" then "Build Android APK", click on the "Create
  template" button. (`AndroidManifest.xml` and "gradle" files are added to the
  project.)

2. In the file `AndroidManifest.xml`, make the following changes using a plain text editor (right-click
on the filename and select "Open With" then "Plain Text Editor").

  - Under the line `<!-- Application arguments -->`, add the following line to
enable ARCore support.

```xml
<meta-data android:name="com.google.ar.core" android:value="required"/>
```

The value can be `optional` or `required`. An AR optional app can be
installed and run on devices that don't support `ARCore`, and the `Google Play
Services for AR` is not automatically installed with the app. An AR required
app is available only on devices that support `ARCore`, and `Google
Play Services for AR` is automatically installed with the app.

For details, see [Google's Enable ARCore documentation](https://developers.google.com/ar/develop/java/enable-arcore#manifest).

  - Under the line `<!-- %%INSERT_FEATURES -->`, add the following line to give
permission to access the camera to ARCore.

```xml
<uses-feature android:name="android.hardware.camera.ar" android:required="true"/>
```

  - Indicate that your app will be checking for the existence of arcore.

```xml
<!-- required at targetSdkVersion=30 to check for arcore support -->
<queries>
  <package android:name="com.google.ar.core"/>
</queries>
```

  - Set the minimum SDK version `android:minSdkVersion` to 24 for AR required app and 14 for
AR optional app.

```xml
<uses-sdk android:minSdkVersion="24" android:targetSdkVersion="28"/>
```

3. In the `build.gradle` file, in `dependencies`, add the ARCore dependency:

```xml
dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar', '*.aar'])
    implementation 'com.google.ar:core:1.11.0'
}
```

4. The project is now ready to be build and run in a compatible Android device.

ARCore requires Android 7.0 or later. The list of compatible devices can be found in
[ARCore supported devices](https://developers.google.com/ar/discover/supported-devices)

## Note for performance issues

There are some conflicts between the AR frameworks and Qt's rendering thread.
See the [SceneView::renderFrame](https://developers.arcgis.com/qt/latest/cpp/api-reference/esri-arcgisruntime-sceneview.html#renderFrame) API documentation and
[Qt's documentation about non-threaded render loops](https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows)
for more information.

The following lines of code enable the non-threaded render loop mode in Qt:

```cpp
#if defined(Q_OS_IOS) && defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```

## Limitations 

AR currently only supports OpenGL. On iOS devices, you must enforce this because the default graphics API on 
iOS devices is Metal. Add the following code to your apps:

```cpp
QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
```
