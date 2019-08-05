
# Augmented Reality (AR) toolkit for ArcGIS Runtime SDK for Qt

The AR toolkit provides support for ARKit for iOS and ARCore for Android.
This documentation describes the installation and uses of the AR toolkik.

## ARKit installation

For the details to uses ARKit, pleases refere to the Apple's documentation: https://developer.apple.com/augmented-reality/

To create an application with the ARKit support:

1. Downloads the sources of the ArcGIS Runtime API Toolkit: https://github.com/Esri/arcgis-runtime-toolkit-qt.
The `AR_TOOLKIT_SOURCE_PATH` variable refers to the path of the sources of the toolkit.

2. Installs the ArcGIS Runtime SDK for Qt version 100.6

3. In Qt Creator, creates a new project "ArcGIS Runtime 100.6 Qt Quick C++ app" or "ArcGIS Runtime 100.6 Qt Quick QML app".

4. In the projet file (.pro), includes the .pri file corresponding to the API used for the project:

```
include($$AR_TOOLKIT_SOURCE_PATH/ArCppApi.pri) // for C++ API
```
or
```
include($$AR_TOOLKIT_SOURCE_PATH/ArQmlApi.pri) // for QML API
```

5. Updates the `Info.plist` file to request persmission for camera:

```
<key>NSCameraUsageDescription</key>
<string>Camera access is needed for AR testing</string>
```

Apple's documentation for `NSCameraUsageDescription`: https://developer.apple.com/documentation/bundleresources/information_property_list/nscamerausagedescription?language=objc

6. The project is ready to be build and run in a iOS device. You need to verify the compatibility of your device
with the ARKit.


## ARCore installation

For the details to uses ARCore, pleases refere to the Google's documentation: https://developers.google.com/ar/
The details of the installation of the ARCore is describes here: https://developers.google.com/ar/develop/c/enable-arcore

To create an application with the ARCore support:

1. Downloads the sources of the ArcGIS Runtime API Toolkit: https://github.com/Esri/arcgis-runtime-toolkit-qt.
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



## Performances

For Android

- https://developer.android.com/ndk/guides/simpleperf
- https://developer.android.com/studio/profile/inspect-gpu-rendering
- https://developer.android.com/studio/profile
- https://developer.android.com/studio/profile/android-profiler

Note for performances issue in mobiles

There are some conflicts between the AR frameworks and the Qt's rendering thread.
This lines of code enable the non-threaded render loops mode in Qt.
See SceneView::renderFrame documentation and Qt's documentation
https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows
for more informations.

```
#if defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif
```
