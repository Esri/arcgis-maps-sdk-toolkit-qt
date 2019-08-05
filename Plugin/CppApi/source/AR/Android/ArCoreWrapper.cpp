/*******************************************************************************
 *  Copyright 2012-2018 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

#include "ArCoreWrapper.h"
#include "arcore_c_api.h"
#include <QtAndroid>
#include "ArcGISArViewInterface.h"
#include <array>
#include "ArCorePointCloudRenderer.h"

#include <QGuiApplication>
#include <QScreen>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

// ArCoreApk_checkAvailability https://developers.google.com/ar/reference/c/group/arcoreapk#group__arcoreapk_1ga448092e9b601bc6f87d13d05317413f1
// ARCore-optional applications must ensure that ArCoreApk_checkAvailability() returns one of the AR_AVAILABILITY_SUPPORTED_... values before calling this method.

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

int32_t ArCoreWrapper::m_installRequested = 1;

namespace {
// Positions of the quad vertices in clip space (X, Y).
const GLfloat kVerticesReversePortrait[] = {
  +1.0f, +1.0f,
  -1.0f, +1.0f,
  +1.0f, -1.0f,
  -1.0f, -1.0f
};
const GLfloat kVerticesRightLandscape[] = {
  -1.0f, +1.0f,
  -1.0f, -1.0f,
  +1.0f, +1.0f,
  +1.0f, -1.0f
};
const GLfloat kVerticesLeftLandscape[] = {
  +1.0f, -1.0f,
  +1.0f, +1.0f,
  -1.0f, -1.0f,
  -1.0f, +1.0f
};
const GLfloat kVerticesPortrait[] = {
  -1.0f, -1.0f,
  +1.0f, -1.0f,
  -1.0f, +1.0f,
  +1.0f, +1.0f
};
}  // namespace

ArCoreWrapper::ArCoreWrapper(ArcGISArViewInterface* arcGISArView) :
  m_arcGISArView(arcGISArView),
  m_arCoreFrameRenderer(this),
  m_arCorePointCloudRenderer(new ArCorePointCloudRenderer(this))
{
  installArCore();
  createArSession();

  // update frames when timer timeout
  QObject::connect(&m_timer, &QTimer::timeout, [this]()
  {
    qDebug() << "----- timer timeout";

    // request the update of the view (in main thread), to render the AR frame (in GL thread).
    m_arcGISArView->update();

    // update the scene view camera
    auto camera = quaternionTranslation();
    m_arcGISArView->updateCamera(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6]);

    // udapte the field of view, based on the
    auto lens = lensIntrinsics();
    m_arcGISArView->updateFieldOfView(lens[0], lens[1], lens[2], lens[3], lens[4], lens[5]);

    // render the frame of the ArcGIS runtime
    m_arcGISArView->renderFrame();
  });

  startTracking();
}

ArCoreWrapper::~ArCoreWrapper()
{
  releasePointCouldData();

  if (m_arFrame)
  {
    ArFrame_destroy(m_arFrame);
    m_arFrame = nullptr;
  }

  if (m_arSession)
  {
    ArSession_destroy(m_arSession);
    m_arSession = nullptr;
  }
}

void ArCoreWrapper::startTracking()
{
  m_timer.start(33);

  if (!m_arSession)
    return;

  ArStatus status = ArSession_resume(m_arSession);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("Fails to resume the AR session.");
    return;
  }
}

void ArCoreWrapper::stopTracking()
{
  m_timer.stop();

  if (!m_arSession)
    return;

  ArStatus status = ArSession_pause(m_arSession);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("Fails to pause the AR session.");
    return;
  }
}

void ArCoreWrapper::resetTracking()
{
  // not implemented
}

void ArCoreWrapper::setSize(const QSize& size)
{
  if (!m_arSession)
    return;

  if (size.width() > 0 && size.height() > 0)
  {
    ArSession_setDisplayGeometry(m_arSession,
                                 0, // ROTATION_0
                                 qMin(size.width(), size.height()),
                                 qMax(size.width(), size.height()));
  }
}

void ArCoreWrapper::setTextureId(GLuint textureId)
{
  m_textureId = textureId;

  if (!m_arSession)
    return;

  // use ArSession_setCameraTextureName to set the texture id created by OpenGL context
  /// Sets the OpenGL texture name (id) that will allow GPU access to the camera
  /// image. The provided ID should have been created with @c glGenTextures(). The
  /// resulting texture must be bound to the @c GL_TEXTURE_EXTERNAL_OES target for
  /// use. Shaders accessing this texture must use a @c samplerExternalOES
  /// sampler. See sample code for an example.
  ArSession_setCameraTextureName(m_arSession, textureId);
}

// this function run on the rendering thread
void ArCoreWrapper::initGL()
{
  m_arCoreFrameRenderer.initGL();

  if (m_arCorePointCloudRenderer)
    m_arCorePointCloudRenderer->initGL();
}

// this function run on the rendering thread
void ArCoreWrapper::render()
{
  udpateArCamera();

  m_arCoreFrameRenderer.render();

  if (m_arCorePointCloudRenderer)
    m_arCorePointCloudRenderer->render();
}

ArSession* ArCoreWrapper::session()
{
  return m_arSession;
}

ArFrame* ArCoreWrapper::frame()
{
  return m_arFrame;
}

//The application's JNIEnv object
JNIEnv* ArCoreWrapper::jniEnvironment()
{
  //  Step 1 get JNIEnv pointer by defining
  //  JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/).
  //  You can define it (once per .so file) in any .cpp file you like


  //  JNIEnv* env;
  //  6 // get the JNIEnv pointer.
  //  7 if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
  //  8 return JNI_ERR;


  //  QAndroidJniEnvironment qjniEnv;
  //  return qjniEnv->ExceptionCheck();
  return m_jniEnvironment;
}

//A jobject referencing the application's current Android Activity.
jobject ArCoreWrapper::applicationActivity()
{
  // Note: AR run in different thread, must to create local reference to avoid deleted object.
  if (m_applicationActivity == nullptr)
    m_applicationActivity = jniEnvironment()->NewLocalRef(QtAndroid::androidActivity().object<jobject>());

  // jobject context = QtAndroid::androidContext().object<jobject>();
  // jobject context_ = env->NewGlobalRef(context);
  // jobject activity_ = env->NewLocalRef(activity);

  return m_applicationActivity;
}

// Initiates installation of ARCore if needed.
// May be called prior to ArSession_create()

// When your apllication launches or enters an AR mode, it should call this method with user_requested_install = 1.
// If ARCore is installed and compatible, this function will set out_install_status to AR_INSTALL_STATUS_INSTALLED.
// If ARCore is not currently installed or the installed version not compatible, the function will set out_install_status to AR_INSTALL_STATUS_INSTALL_REQUESTED and return immediately. Your current activity will then pause while the user is offered the opportunity to install it.
// When your activity resumes, you should call this method again, this time with user_requested_install = 0. This will either set out_install_status to AR_INSTALL_STATUS_INSTALLED or return an error code indicating the reason that installation could not be completed.

bool ArCoreWrapper::installArCore()
{
  if (m_arSession != nullptr)
    return true;

  ArInstallStatus installStatus;
  ArStatus error = ArCoreApk_requestInstall(jniEnvironment(), applicationActivity(), m_installRequested, &installStatus);
  if (error != AR_SUCCESS)
  {
    //AR_ERROR_FATAL if an error occurs while checking for or requesting installation
    //AR_UNAVAILABLE_DEVICE_NOT_COMPATIBLE if ARCore is not supported on this device.
    //AR_UNAVAILABLE_USER_DECLINED_INSTALLATION if the user previously declined installation.
    // see https://stackoverflow.com/questions/49808178/android-nativeactivity-and-arcore-c-lib-function-arsession-create-fails-whe
    // aar file: https://mvnrepository.com/artifact/com.google.ar/core/1.9.0
    return false;
  }

  switch (installStatus)
  {
    case AR_INSTALL_STATUS_INSTALLED:
      // The requested resource is already installed.
      return true;
    case AR_INSTALL_STATUS_INSTALL_REQUESTED:
      // Installation of the resource was requested. The current activity will be paused.
      m_installRequested = 0;
      return false;
  }
}

// java: public static native long createNativeApplication(AssetManager assetManager);

// java: JniInterface.onResume(nativeApplication, getApplicationContext(), this);
// java: public static native void onResume(long nativeApplication, Context context, Activity activity);
void ArCoreWrapper::createArSession()
{
  if (m_arSession)
    return;

  // === ATTENTION!  ATTENTION!  ATTENTION! ===
  // This method can and will fail in user-facing situations.  Your
  // application must handle these cases at least somewhat gracefully.  See
  // HelloAR Java sample code for reasonable behavior.
  auto status = ArSession_create(jniEnvironment(), applicationActivity(), &m_arSession);
  if (status != AR_SUCCESS || !m_arSession)
  {
    emit m_arcGISArView->errorOccurred("Fails to create the AR session.");
    return;
  }

  // request camera permission
  const QString permissionKey = QStringLiteral("android.permission.CAMERA");
  auto permissions = QtAndroid::requestPermissionsSync({ permissionKey }, 5000);
  if (permissions[permissionKey] != QtAndroid::PermissionResult::Granted)
  {
    emit m_arcGISArView->errorOccurred("Fails to access to the camera.");
    return;
  }

  // === ATTENTION!  ATTENTION!  ATTENTION! ===
  // This method can and will fail in user-facing situations.  Your
  // application must handle these cases at least somewhat gracefully.  See
  // HelloAR Java sample code for reasonable behavior.
  ArConfig* arConfig = nullptr;
  ArConfig_create(m_arSession, &arConfig);
  if (arConfig == nullptr)
  {
    emit m_arcGISArView->errorOccurred("Fails to create the AR config.");
    return;
  }

  ArConfig_setFocusMode(m_arSession, arConfig, AR_FOCUS_MODE_AUTO);
  status = ArSession_configure(m_arSession, arConfig);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("Fails to configure the AR session.");
    return;
  }

  ArConfig_destroy(arConfig);
}

void ArCoreWrapper::udpateArCamera()
{
  if (!m_arSession)
    return;

  // release data if necessary
  if (m_arCamera)
  {
    ArCamera_release(m_arCamera);
    m_arCamera = nullptr;
  }

  // create and update AR frame
  if (!m_arFrame)
  {
    ArFrame_create(m_arSession, &m_arFrame);
    if (!m_arFrame)
    {
      emit m_arcGISArView->errorOccurred("Fails to create an AR frame.");
      return;
    }
  }

  // note: message from Android team
  //  Luke Duncan [2:00 AM]
  //  I have applied @gunt0001’s work to the `ArcGISArView` and I’ve found that there’s a decrease in FPS when it’s activated.
  //  In the video we have manual rendering *off* -> manual rendering *on* -> manual rendering *off*.

  //  This is likely down to the fact that the `onUpdate()` method, that informs us of the new frame, is run on the
  //  main thread. So to allow the current implementation to not block the thread, we need to call `post()` with the
  //  `Runnable` we wish to execute. This adds this call to `renderFrame()` onto a message queue so it isn’t quite
  //  in sync with the `onUpdate()` method.

  ArStatus status = ArSession_update(m_arSession, m_arFrame);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("Fails to update the AR frame.");
    return;
  }

  // If display rotation changed (also includes view size change), we need to
  // re-query the uv coordinates for the on-screen portion of the camera image.
  int32_t geometryChanged = 0;
  ArFrame_getDisplayGeometryChanged(m_arSession, m_arFrame, &geometryChanged);
  if (geometryChanged != 0 || !m_uvsInitialized)
  {
    // get the screen orientation
    // todo: move to ArCoreUtils
    const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

    switch (orientation) {
      case Qt::PortraitOrientation:
        ArFrame_transformCoordinates2d(
              m_arSession, m_arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
              kNumVertices, kVerticesPortrait, AR_COORDINATES_2D_TEXTURE_NORMALIZED,
              m_transformedUvs);
        break;
      case Qt::LandscapeOrientation:
        ArFrame_transformCoordinates2d(
              m_arSession, m_arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
              kNumVertices, kVerticesLeftLandscape, AR_COORDINATES_2D_TEXTURE_NORMALIZED,
              m_transformedUvs);
        break;
      case Qt::InvertedPortraitOrientation:
        ArFrame_transformCoordinates2d(
              m_arSession, m_arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
              kNumVertices, kVerticesReversePortrait, AR_COORDINATES_2D_TEXTURE_NORMALIZED,
              m_transformedUvs);
        break;
      case Qt::InvertedLandscapeOrientation:
        ArFrame_transformCoordinates2d(
              m_arSession, m_arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
              kNumVertices, kVerticesRightLandscape, AR_COORDINATES_2D_TEXTURE_NORMALIZED,
              m_transformedUvs);
        break;
      default:
        break;
    }

    m_uvsInitialized = true;
  }

  int64_t timestamp = 0;
  ArFrame_getTimestamp(m_arSession, m_arFrame, &timestamp);
  if (timestamp == 0)
  {
    // Suppress rendering if the camera did not produce the first frame yet.
    // This is to avoid drawing possible leftover data from previous sessions if
    // the texture is reused.
    return;
  }

  ArFrame_acquireCamera(m_arSession, m_arFrame, &m_arCamera);
  if (!m_arCamera)
  {
    emit m_arcGISArView->errorOccurred("Fails to acquire the camera.");
    return;
  }
}

// The returned array contains the following parameters: the quaternions x, y, z and w
// end the translations x, y and z. These parameters can be used to create a transformation
// matrix object using the "createWithQuaternionAndTranslation" function.
std::array<double, 7> ArCoreWrapper::quaternionTranslation() const
{
  ArPose* pose = nullptr;
  ArPose_create(m_arSession, nullptr, &pose);
  if (!pose)
  {
    emit m_arcGISArView->errorOccurred("Fails to create an AR pose.");
    return {};
  }

  float poseRaw[7] = {};
  ArCamera_getDisplayOrientedPose(m_arSession, m_arCamera, pose);
  ArPose_getPoseRaw(m_arSession, pose, poseRaw);
  ArPose_destroy(pose);

  // get the screen orientation
  // todo: move to ArCoreUtils
  const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

  switch (orientation) {
    case Qt::PortraitOrientation:

      return {
        poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6]
      };

      break;
    case Qt::LandscapeOrientation:
    {
      qDebug() << "==== LandscapeOrientation";
      const double x =  0 * poseRaw[3] + 0 * poseRaw[2] + 0.70710678118 * poseRaw[1] + 0.70710678118 * poseRaw[0];
      const double y = -0 * poseRaw[2] + 0 * poseRaw[3] - 0.70710678118 * poseRaw[0] + 0.70710678118 * poseRaw[1];
      const double z =  0 * poseRaw[1] - 0 * poseRaw[0] - 0.70710678118 * poseRaw[3] + 0.70710678118 * poseRaw[2];
      const double w = -0 * poseRaw[0] - 0 * poseRaw[1] + 0.70710678118 * poseRaw[2] + 0.70710678118 * poseRaw[3];

      return {
        x, y, z, w, poseRaw[4], poseRaw[5], poseRaw[6]
      };

      break;
    }
    case Qt::InvertedPortraitOrientation:
      break;
    case Qt::InvertedLandscapeOrientation:
    {
      qDebug() << "==== InvertedLandscapeOrientation";
      const double x =  0 * poseRaw[3] + 0 * poseRaw[2] - 0.70710678118 * poseRaw[1] + 0.70710678118 * poseRaw[0];
      const double y = -0 * poseRaw[2] + 0 * poseRaw[3] + 0.70710678118 * poseRaw[0] + 0.70710678118 * poseRaw[1];
      const double z =  0 * poseRaw[1] - 0 * poseRaw[0] + 0.70710678118 * poseRaw[3] + 0.70710678118 * poseRaw[2];
      const double w = -0 * poseRaw[0] - 0 * poseRaw[1] - 0.70710678118 * poseRaw[2] + 0.70710678118 * poseRaw[3];

      return {
        x, y, z, w, poseRaw[4], poseRaw[5], poseRaw[6]
      };

      break;
    }
    default:
      break;
  }

  return {
    poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6]
  };
}

// The returned array contains the following parameters: xFocalLength, yFocalLength,
// xPrincipal, yPrincipal, xImageSize and yImageSize.
// These parameters can be used to set the field of view in the scene view with the
// "setFieldOfViewFromLensIntrinsics" function.
std::array<double, 6> ArCoreWrapper::lensIntrinsics() const
{
  ArCameraIntrinsics* cameraIntrinsics = nullptr;
  ArCameraIntrinsics_create(m_arSession, &cameraIntrinsics);
  if (!cameraIntrinsics)
  {
    emit m_arcGISArView->errorOccurred("Fails to create an AR camera instrinsics.");
    return {};
  }

  ArCamera_getImageIntrinsics(m_arSession, m_arCamera, cameraIntrinsics);

  float xFocalLength = 0.0f;
  float yFocalLength = 0.0f;
  ArCameraIntrinsics_getFocalLength(m_arSession, cameraIntrinsics, &xFocalLength, &yFocalLength);

  float xPrincipal = 0.0f;
  float yPrincipal = 0.0f;
  ArCameraIntrinsics_getPrincipalPoint(m_arSession, cameraIntrinsics, &xPrincipal, &yPrincipal);

  int32_t xImageSize = 0;
  int32_t yImageSize = 0;
  ArCameraIntrinsics_getImageDimensions(m_arSession, cameraIntrinsics, &xImageSize, &yImageSize);

  ArCameraIntrinsics_destroy(cameraIntrinsics);

  return {
    static_cast<double>(xFocalLength), static_cast<double>(yFocalLength),
    static_cast<double>(xPrincipal), static_cast<double>(yPrincipal),
    static_cast<double>(xImageSize), static_cast<double>(yImageSize)
  };
}

const float* ArCoreWrapper::transformedUvs() const
{
  return m_transformedUvs;
}

// Methods for point cloud data.
// Be careful with the concurrent access and lifetime: the data are initialized and
// released in the main thread and used in the GL thread.
// todo: mutex lock for access to m_arFrame?
void ArCoreWrapper::pointCloudData(QMatrix4x4& mvp, int32_t& size, const float** data)
{
  if (!m_renderPointCloud || m_arPointCloud)
    return;

  // get the point cloud data
  ArStatus pointCloudStatus = ArFrame_acquirePointCloud(m_arSession, m_arFrame, &m_arPointCloud);
  if (pointCloudStatus != AR_SUCCESS)
  {
    releasePointCouldData();
    return;
  }

  ArPointCloud_getNumberOfPoints(m_arSession, m_arPointCloud, &size);
  if (size <= 0)
  {
    releasePointCouldData();
    return;
  }

  ArPointCloud_getData(m_arSession, m_arPointCloud, data);

  // get the model view projection matrix
  QMatrix4x4 viewMatrix;
  ArCamera_getViewMatrix(m_arSession, m_arCamera, viewMatrix.data());

  QMatrix4x4 projectionMatrix;
  ArCamera_getProjectionMatrix(m_arSession, m_arCamera, 0.1f, 100.f, projectionMatrix.data());

  mvp = projectionMatrix * viewMatrix;
}

// The point cloud data can be released after the rendering is done.
void ArCoreWrapper::releasePointCouldData()
{
  if (!m_arPointCloud)
    return;

  ArPointCloud_release(m_arPointCloud);
  m_arPointCloud = nullptr;
}
