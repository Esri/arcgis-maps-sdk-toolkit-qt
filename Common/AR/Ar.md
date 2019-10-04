
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
and when continuous GPS tracking is required.

The minimal version of the ArcGIS Runtime SDK for Qt is 100.6.

### Features of the AR component

- Allows display of the live camera feed
- Manages `ARKit` or `ARCore` session lifecycle
- Tracks user location and device orientation through a combination of `ARKit`or `ARCore` and the device GPS
- Provides access to a `SceneView` to display your GIS 3D data over the live camera feed
- `screenToLocation` method to convert a screen point to a real-world coordinate
- Easy access to all `ARKit` or `ARCore` and `LocationDataSource` delegate methods

## ARKit installation

For details about using `ARKit`, please refer to [Apple's documentation](https://developer.apple.com/augmented-reality).

To create an application with the `ARKit` support:

1. Download the sources of the [ArcGIS Runtime API Toolkit](https://github.com/Esri/arcgis-runtime-toolkit-qt).
Set the `AR_TOOLKIT_SOURCE_PATH` variable to the path of the toolkit sources.

2. Install the ArcGIS Runtime SDK for Qt.

3. In Qt Creator, create a new project "ArcGIS Runtime 100.x Qt Quick C++ app" or "ArcGIS Runtime 100.x Qt Quick QML app".

4. In the project file (.pro), include the .pri file corresponding to the API used for the project:

```
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/ArCppApi.pri) // for C++ API
```
or
```
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/ArQmlApi.pri) // for QML API
```

5. Update the `Info.plist` file to request permission for camera:

```
<key>NSCameraUsageDescription</key>
<string>Camera access is needed for AR testing</string>
```

See [NSCameraUsageDescription Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/nscamerausagedescription?language=objc) for details.

6. The project is ready to be build and run in an iOS device. You need to verify the compatibility of your device
with the ARCore.

## ARCore installation

For details about using `ARCore`, please refer to [Google's documentation](https://developers.google.com/ar/).
Installation details of `ARCore` are described at
[documentation](https://developers.google.com/ar/develop/c/enable-arcore).

To create an application with the `ARCore` support:

1. Download the sources of the [ArcGIS Runtime API Toolkit](https://github.com/Esri/arcgis-runtime-toolkit-qt).
The `AR_TOOLKIT_SOURCE_PATH` variable refers to the toolkit sources path.

2. Install the ArcGIS Runtime SDK for Qt version.

3. In Qt Creator, create a new project "ArcGIS Runtime 100.x Qt Quick C++ app" or "ArcGIS Runtime 100.x Qt Quick QML app".

4. In the project file (.pro), include the `.pri` file corresponding to the API used by the project:

```
include($$AR_TOOLKIT_SOURCE_PATH/ArCppApi.pri) // for C++ API
```
or
```
include($$AR_TOOLKIT_SOURCE_PATH/ArQmlApi.pri) // for QML API
```

5. Copy the gradle files from the ARCore SDK for Android to the `Android` dir in our project.

6. In the Android manifest file `AndroidManifest.xml`, add the following entries:

```
<meta-data android:name="com.google.ar.core" android:value="required" />
```

The value can be `optional` or `required`. An AR optional app can be installed and run on devices
that don't support `ARCore` and the `Google Play Services for AR` is not automatically installed
with the app. An AR required app is available only on devices that support ARCore and the
`Google Play Services for AR` is automatically installed.

For details, see [Enable ARCore documentation](https://developers.google.com/ar/develop/java/enable-arcore#manifest).

7. Verify that the permission for camera is present:

```
<uses-permission android:name="android.permission.CAMERA"/>
```

7. The project is ready to be built and run on an Android device. You need to verify the compatibility of your device
with the ARKit.

## Usage

Using the C++ API to explore a scene in AR:

```
// create the AR view
auto arcGISArView = new ArcGISArView(this);
auto sceneView = new SceneQuickView(this);
arcGISArView->setSceneView(sceneView);

// Create a simple scene with a transparency background.
auto scene = new Scene(BasemapType::ImageryWithLabels, this);
BackgroundGrid grid;
grid.setVisible(false);
auto baseSurface = new Surface(this);
baseSurface->setBackgroundGrid(grid);
baseSurface->setNavigationConstraint(NavigationConstraint::None);
scene->setBaseSurface(baseSurface);
sceneView->setArcGISScene(scene);

// Set a LocationDataSource, used to get our initial real-world location.
arcGISArView->setLocationDataSource(new LocationDataSource(this));

// Start tracking our location and device orientation
arcGISArView->startTracking();
```

Using the QML API to explore a scene in AR:

```
// create the AR view
ArcGISArView {
    id: arcGISArView
    anchors.fill: parent
    sceneView: sceneView
    locationDataSource: LocationDataSource {}
}

SceneView {
    id: sceneView
    anchors.fill: parent
    scene: scene
    onMousePressed: arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen events
}

// Create a simple scene with a transparency background.
Scene {
    id: scene
    Surface {
        navigationConstraint: Enums.NavigationConstraintNone
        backgroundGrid: BackgroundGrid {
            visible: false
        }
    }
}
```

To see it in action, try out the `CppArExample` and `QmlArExample` in the project.

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
