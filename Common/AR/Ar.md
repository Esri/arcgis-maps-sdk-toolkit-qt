
# Augmented Reality (AR) toolkit for ArcGIS Runtime SDK for Qt

The AR toolkit provides support for ARKit for iOS and ARCore for Android.
This documentation describes the installation and uses of the AR toolkik.

## Overview

Augmented reality experiences are designed to "augment" the physical world with virtual content that
respects real world scale, position, and orientation of a device. In the case of Runtime, a SceneView
displays 3D geographic data as virtual content on top of a camera feed which represents the real, physical
world.

The Augmented Reality (AR) toolkit component allows quick and easy integration of AR into your application
for a wide variety of scenarios. The toolkit recognizes the following common patterns for AR:

- **Flyover**: Flyover AR allows you to explore a scene using your device as a window into the virtual world.
A typical flyover AR scenario will start with the scene’s virtual camera positioned over an area of interest.
You can walk around and reorient the device to focus on specific content in the scene.
- **Tabletop**: Scene content is anchored to a physical surface, as if it were a 3D-printed model.
- **Real-scale**: Scene content is rendered exactly where it would be in the physical world. A camera feed
is shown and GIS content is rendered on top of that feed. This is used in scenarios ranging from viewing
hidden infrastructure to displaying waypoints for navigation.

The AR toolkit component is comprised of one class: `ArcGISArView`. This is a subclass of `QQuickItem` that
contains the functionality needed to display an AR experience in your application. It uses `ARKit` (Apple) or
`ARCore` (Android) augmented reality framework to display the live camera feed and handle real world tracking
and synchronization with the Runtime SDK's `SceneView`. The `ArcGISArView` is responsible for starting and
managing an `ARKit` or `ARCore` session. It uses a `LocationDataSource` for getting an initial GPS location
and when continuous GPS tracking is required.

### Features of the AR component

- Allows display of the live camera feed
- Manages `ARKit` or `ARCore` session lifecycle
- Tracks user location and device orientation through a combination of `ARKit`or `ARCore` and the device GPS
- Provides access to an `SceneView` to display your GIS 3D data over the live camera feed
- `screenToLocation` method to convert a screen point to a real-world coordinate
- Easy access to all `ARKit` or `ARCore` and `LocationDataSource` delegate methods

## ARKit installation

For the details to uses `ARKit`, pleases refere to the [Apple's documentation](https://developer.apple.com/augmented-reality).

To create an application with the `ARKit` support:

1. Downloads the sources of the [ArcGIS Runtime API Toolkit](https://github.com/Esri/arcgis-runtime-toolkit-qt).
Sets the `AR_TOOLKIT_SOURCE_PATH` variable to the path of the sources of the toolkit.

2. Installs the ArcGIS Runtime SDK for Qt version 100.6

3. In Qt Creator, creates a new project "ArcGIS Runtime 100.6 Qt Quick C++ app" or "ArcGIS Runtime 100.6 Qt Quick QML app".

4. In the projet file (.pro), includes the .pri file corresponding to the API used for the project:

```
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/CppApi/ArCppApi.pri) // for C++ API
```
or
```
include($$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/ArQmlApi.pri) // for QML API
```

5. Updates the `Info.plist` file to request persmission for camera:

```
<key>NSCameraUsageDescription</key>
<string>Camera access is needed for AR testing</string>
```

See [NSCameraUsageDescription Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/nscamerausagedescription?language=objc) for details.

6. The project is ready to be build and run in an iOS device. You need to verify the compatibility of your device
with the ARKit.

## ARCore installation

For the details to uses `ARCore`, pleases refere to the [Google's documentation](https://developers.google.com/ar/).
The details of the installation of the `ARCore` is describes in the
[documentation](https://developers.google.com/ar/develop/c/enable-arcore).

To create an application with the `ARCore` support:

1. Downloads the sources of the [ArcGIS Runtime API Toolkit: https://github.com/Esri/arcgis-runtime-toolkit-qt.
The `AR_TOOLKIT_SOURCE_PATH` variable refers to the path of the sources of the toolkit.

2. Downloads and extracts the ARCore SDK for Android v1.10: https://github.com/google-ar/arcore-android-sdk/releases.
This project contains the C header file (`arcore_c_api.h`) used to build the ARCore wrapper.
The `AR_CORE_HEADER_PATH` variable refers to the path of the ARCore header files.

3. Downloads the ARCore `aar` file from the Maven repository: https://maven.google.com/com/google/ar/core/1.10.0/core-1.10.0.aar
This archive contains the native libraries used by the ARCore wrapper. Renames the extention file from `aar` to `zip` and
extracts it.
The `AR_CORE_BINARIES_PATH` variable refers to the path of the files of the ARCore native libraries.

4. Installs the ArcGIS Runtime SDK for Qt version 100.6

5. In Qt Creator, creates a new project "ArcGIS Runtime 100.6 Qt Quick C++ app" or "ArcGIS Runtime 100.6 Qt Quick QML app".

6. In the projet file (.pro), includes the .pri file corresponding to the API used for the project:

```
include($$AR_TOOLKIT_SOURCE_PATH/ArCppApi.pri) // for C++ API
```
or
```
include($$AR_TOOLKIT_SOURCE_PATH/ArQmlApi.pri) // for QML API
```

7. Copy the gradle files from the ARCore SDK for Android to the `Android` dir in our project.

8. In the Android manifest file `AndroidManifest.xml`, adds the following entries:

```
<meta-data android:name="com.google.ar.core" android:value="optional" />
```

Verifies the permission for camera is present:

```
<uses-permission android:name="android.permission.CAMERA"/>
```

9. The project is ready to be build and run in an Android device. You need to verify the compatibility of your device
with the ARKit.

## Usage

Using the C++ API:

```
// create the AR view
auto arcGISArView = new ArcGISArView(this);
auto sceneView = new SceneQuickView(this);
arcGISArView->setSceneView(ceneView);

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

Using the QML API:

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
    scene: sceneLoader.item ? sceneLoader.item.scene : null
    onMousePressed: arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen event
}

// Create a simple scene with a transparency background.
Scene {
    Surface {
        navigationConstraint: Enums.NavigationConstraintNone
        backgroundGrid: BackgroundGrid {
            visible: false
        }
    }
}
```

To see it in action, try out the `CppArExample` and `AmlArExample` in the project.

## Note for performances issue in mobiles

There are some conflicts between the AR frameworks and the Qt's rendering thread.
This lines of code enable the non-threaded render loops mode in Qt.
See `SceneView::renderFrame` documentation and
[Qt's documentation](https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows)
for more informations.

```
#if defined(Q_OS_IOS) && defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```
