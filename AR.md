
// notes:
// - default values?
// - inheritance from SceneView?


# Demo AR

Demo to test ARCore (Google) and ARKit (Apple).

"Augmented reality (AR) describes user experiences that add 2D or 3D elements to
the live view from a device's camera in a way that makes those elements appear to
inhabit the real world." (from ARKit documentation)

Issue: https://devtopia.esri.com/runtime/qt-common/issues/4525

## Concepts

The ARs runtimes take the control of the cameras and the sensors directly, it's not necessary to
create `Camera` or `Sensor` classes and pass them to AR runtimes. The only thing to do is to create
the AR runtimes and check if AR is supported on the device.

2 types of apps:
- "AR Required". The app is not usable without AR and is available only on devices that support AR.
- "AR Optional". The app includes AR feature that is activated if the device supports AR.


"session"

"configuration"

"anchor"



## Google ARCore SDK

- main URL: https://developers.google.com/ar/
- supported platformes: Android SDK (Java), Android NDK (C++), iOS (cloud anchor), Unity, Unreal.
- minimal Android SDK version: 14 (AR optional) or 24 (AR required)
- this demo focus on NDK: https://developers.google.com/ar/develop/c/quickstart

### Runtime considerations

ARCore's Motion Tracking. 
Visual information from the camera and inertial measurements.

UX design:
- Provides clear feedback to users
- Encourages them to move their device, and slowly. Images become blurry, reducing ARCore's ability to track and detect features.
- Shows them how to interact with their device to experience AR

Make effective use of anchors
Avoid using haptic feedback

#### How anchors work

World space
- Coordinate space in which the camera and objects are positioned
- Camera and object positions are updated in world space from frame to frame

Pose
- Represents an object’s position and orientation in world space

Use anchors in your scene
- Create anchors in the context of a Trackable (such as a Plane) or the ARCore Session.
- Attach one or more objects to the anchor.

Trackable or the ARCore Session

### Install and debug

"Ninja" lib must be installed on the computer. To install on Mac: `brew install ninja`.

This command line can be used to debug the Gradle errors (errors in deployment):

```
./gradlew build --stacktrace
```

### Build

#### Android Manifest

The AR runtime need to access to the camera. To give the permission for camera, add this line
in the Android manifest:

```
<uses-permission android:name="android.permission.CAMERA"/>
```

The documentation for permission: https://developer.android.com/training/permissions/requesting

checks permission (java): cf CameraPermissionHelper.java in hello_ar_c project

```
/** Check to see we have the necessary permissions for this app. */
public static boolean hasCameraPermission(Activity activity) {
  return ContextCompat.checkSelfPermission(activity, CAMERA_PERMISSION)
      == PackageManager.PERMISSION_GRANTED;
}
```

## Qt5:

`QtAndroid::checkPermission` http://doc.qt.io/qt-5/qtandroid.html#checkPermission

Camera: http://doc.qt.io/qt-5/cameraoverview.html

- feature for ARCore
- feature for camera.ar

```
  <!-- This tag indicates that this application requires ARCore.  This results in the application
       only being visible in the Google Play Store on devices that support ARCore. -->
  <uses-feature android:name="android.hardware.camera.ar" android:required="true"/>
  <uses-feature android:glEsVersion="0x00020000" android:required="true" />
```

- lib ARCore

```
    <!-- This tag indicates that this application requires ARCore.  This results in the Google Play
          Store downloading and installing ARCore along with the application. -->
    <meta-data android:name="com.google.ar.core" android:value="required" />
```



#### C++ code

get camera infos: see hello_ar_application.cc, starts in line 148

Qt3D ?

install ARCore on Android



## Apple ARKit 2

- website: https://developer.apple.com/arkit/

Minimal configuration: "All ARKit configurations require an iOS device with an A9 or later processor."

### ARKit

"A curated list of awesome ARKit projects and resources": https://github.com/olucurious/Awesome-ARKit

#### Verifying Device Support and User Permission

https://developer.apple.com/documentation/arkit/verifying_device_support_and_user_permission?language=objc

> If the basic functionality of your app requires AR (using the back camera): Add the arkit key in the UIRequiredDeviceCapabilities section of your app's Info.plist file. Using this key makes your app available only to ARKit-compatible devices.

> If augmented reality is a secondary feature of your app: Check for whether the current device supports the AR configuration you want to use by testing the isSupported property of the appropriate ARConfiguration subclass.

- face-tracking AR

+ grant the access to app. Your app's Info.plist file must include the NSCameraUsageDescription key.

### Session

```
@interface ARSession : NSObject

- (void)runWithConfiguration:(ARConfiguration *)configuration;
```

> reading data from the device's motion sensing hardware, controlling the device's built-in camera, and performing image analysis on captured camera image

1 instance of ARSession. Already include in ARSCNView and ARSKView

```
/* Abstract class */
@interface ARConfiguration : NSObject 

/* Concret classes */
@interface ARWorldTrackingConfiguration : ARConfiguration
@interface AROrientationTrackingConfiguration : ARConfiguration
@interface ARImageTrackingConfiguration : ARConfiguration
@interface ARFaceTrackingConfiguration : ARConfiguration
@interface ARObjectScanningConfiguration : ARConfiguration

@property(class, nonatomic, readonly) BOOL isSupported;
```

Example:
```
@property (nonatomic, strong) IBOutlet ARSCNView *sceneView;


// Create a session configuration
ARWorldTrackingConfiguration *configuration = [ARWorldTrackingConfiguration new];

// Run the view's session
[self.sceneView.session runWithConfiguration:configuration];

// Pause the view's session
[self.sceneView.session pause];
```


### Delegate

Use ARSession.delegate property. Create a class derived from ARSessionDelegate, override the function didUpdateFrame. This function is called every time the AR session analyze a frame.

@property(nonatomic, weak) id<ARSessionDelegate> delegate;

Doc:
- delegate function: https://developer.apple.com/documentation/arkit/arsession/2865614-delegate?language=objc
- ARSessionDelegate class: https://developer.apple.com/documentation/arkit/arsessiondelegate?language=objc
- didUpdateFrame function: https://developer.apple.com/documentation/arkit/arsessiondelegate/2865611-session?language=objc 

Code example (Objective-C):
```
@interface MyDelegate : NSObject<ARSessionDelegate>

-(void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame;

@end

@implementation MyDelegate

-(void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame
{
    NSLog(@"didUpdateFrame is called.");
}

@end

// create delegate
MyDelegate* myDelegate = [[MyDelegate alloc]init];
arSession.delegate = myDelegate;
```

The `frame` parameter can be used to get the matrix tranform.

```
simd_float4x4 projectionMatrix = frame.camera.projectionMatrix;
```

Useful properties:
- `projectionMatrix`: "A transform matrix appropriate for rendering 3D content to match the image captured by the camera." [simd_float4x4]
- `transform`: "The position and orientation of the camera in world coordinate space." [simd_float4x4]
- `eulerAngles`: "The orientation of the camera, expressed as roll, pitch, and yaw values." [simd_float3]

Doc:
- ARFrame class: https://developer.apple.com/documentation/arkit/arframe?language=objc
- ARCamera class: https://developer.apple.com/documentation/arkit/arcamera?language=objc
- simd_float4x4 structure: https://developer.apple.com/documentation/simd/simd_float4x4?language=objc
- simd_float3 structure: https://developer.apple.com/documentation/simd/simd_float3?language=objc



### Integrating Objective-C in C++

http://doc.qt.io/qt-5/ios.html

>  To enable this mode, suffix your source files with .mm, and add them to OBJECTIVE_SOURCES instead of SOURCES in the .pro 

to add ARKit framework in .pro:

```
LIBS += -framework ARKit
```







https://forum.qt.io/topic/94091/qt-ar-why-and-how-to-add-augmented-reality-to-your-mobile-app

https://forum.qt.io/topic/2154/augmented-reality-in-qt/4

https://v-play.net/cross-platform-development/qt-ar-why-and-how-to-add-augmented-reality-to-your-mobile-app#why-use-wikitude-instead-of-arkit-or-arcore

https://github.com/aseba-community/qt-ar-demo
















