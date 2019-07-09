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

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

// TODO?

// ArCoreApk_checkAvailability https://developers.google.com/ar/reference/c/group/arcoreapk#group__arcoreapk_1ga448092e9b601bc6f87d13d05317413f1
// ARCore-optional applications must ensure that ArCoreApk_checkAvailability() returns one of the AR_AVAILABILITY_SUPPORTED_... values before calling this method.

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

int32_t ArCoreWrapper::m_installRequested = 1;

namespace {
// Positions of the quad vertices in clip space (X, Y).
const GLfloat kVertices[] = {
  -1.0f, -1.0f,
  +1.0f, -1.0f,
  -1.0f, +1.0f,
  +1.0f, +1.0f,
};
}  // namespace

ArCoreWrapper::ArCoreWrapper(ArcGISArViewInterface* /*arcGISArView*/) :
  m_arCoreFrameRenderer(this),
  m_arCorePointCloudRenderer(this)
{
  install();
  create();
}

ArCoreWrapper::~ArCoreWrapper()
{
  destroy(); // destroy here or in pause()? -> need to recreate session and frame in resume() in this case.
}

void ArCoreWrapper::startTracking()
{
  resume();
}

void ArCoreWrapper::stopTracking()
{
  pause();
}

void ArCoreWrapper::setSize(const QSize& size)
{
  if (size.width() > 0 && size.height() > 0)
  {
    ArSession_setDisplayGeometry(m_arSession, 0 /*rotation*/, size.width(), size.height());
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

void ArCoreWrapper::init()
{
  m_arCoreFrameRenderer.init();
  m_arCorePointCloudRenderer.init();
}

void ArCoreWrapper::render()
{
  beforeRendering();
  m_arCoreFrameRenderer.render();
  m_arCorePointCloudRenderer.render();
  afterRendering();
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

bool ArCoreWrapper::install()
{
  if (m_arSession != nullptr)
    return true;

  ArInstallStatus install_status;
  ArStatus error = ArCoreApk_requestInstall(jniEnvironment(), applicationActivity(), m_installRequested, &install_status);
  if (error != AR_SUCCESS)
  {
    //AR_ERROR_FATAL if an error occurs while checking for or requesting installation
    //AR_UNAVAILABLE_DEVICE_NOT_COMPATIBLE if ARCore is not supported on this device.
    //AR_UNAVAILABLE_USER_DECLINED_INSTALLATION if the user previously declined installation.
    // see https://stackoverflow.com/questions/49808178/android-nativeactivity-and-arcore-c-lib-function-arsession-create-fails-whe
    // aar file: https://mvnrepository.com/artifact/com.google.ar/core/1.9.0
    return false;
  }

  switch (install_status)
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
void ArCoreWrapper::create()
{
  if (m_arSession != nullptr)
    return;

  // === ATTENTION!  ATTENTION!  ATTENTION! ===
  // This method can and will fail in user-facing situations.  Your
  // application must handle these cases at least somewhat gracefully.  See
  // HelloAR Java sample code for reasonable behavior.
  auto status = ArSession_create(jniEnvironment(), applicationActivity(), &m_arSession);
  if (status != AR_SUCCESS)
  {
    qDebug() << "ArSession_create fails.";
    return;
  }

  // config camera

  // === ATTENTION!  ATTENTION!  ATTENTION! ===
  // This method can and will fail in user-facing situations.  Your
  // application must handle these cases at least somewhat gracefully.  See
  // HelloAR Java sample code for reasonable behavior.
  ArConfig* ar_config = nullptr;
  ArConfig_create(m_arSession, &ar_config);
  if (ar_config == nullptr)
  {
    qDebug() << "ArSession_create fails.";
    return;
  }

  ArConfig_setFocusMode(m_arSession, ar_config, AR_FOCUS_MODE_AUTO);
  status = ArSession_configure(m_arSession, ar_config);
  if (status != AR_SUCCESS)
  {
    qDebug() << "ArSession_configure fails.";
    return;
  }

  ArConfig_destroy(ar_config);
}

void ArCoreWrapper::pause()
{
  if (!m_arSession)
    return;

  ArStatus status = ArSession_pause(m_arSession);
  if (status != AR_SUCCESS)
  {
    qDebug() << "ArSession_create fails.";
    return;
  }
}

void ArCoreWrapper::resume()
{
  if (!m_arSession)
    return;

  ArStatus status = ArSession_resume(m_arSession);
  if (status != AR_SUCCESS)
  {
    qDebug() << "ArSession_create fails.";
    return;
  }
}

void ArCoreWrapper::beforeRendering()
{
  if (!m_arSession)
    return;

  // create and update AR frame
  ArFrame_create(m_arSession, &m_arFrame);
  if (!m_arFrame)
    qDebug() << "m_arFrame is nullptr";



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
    qDebug() << "ArSession_update fails";
    return;
  }

  static_assert(std::extent<decltype(kVertices)>::value == kNumVertices * 2,
                "Incorrect kVertices length");

  // If display rotation changed (also includes view size change), we need to
  // re-query the uv coordinates for the on-screen portion of the camera image.
  int32_t geometry_changed = 0;
  ArFrame_getDisplayGeometryChanged(m_arSession, m_arFrame, &geometry_changed);
  if (geometry_changed != 0 || !uvs_initialized_)
  {
    ArFrame_transformCoordinates2d(
          m_arSession, m_arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
          kNumVertices, kVertices, AR_COORDINATES_2D_TEXTURE_NORMALIZED,
          m_transformedUvs);
    uvs_initialized_ = true;
  }

  int64_t frame_timestamp = 0;
  ArFrame_getTimestamp(m_arSession, m_arFrame, &frame_timestamp);
  if (frame_timestamp == 0)
  {
    // Suppress rendering if the camera did not produce the first frame yet.
    // This is to avoid drawing possible leftover data from previous sessions if
    // the texture is reused.
    return;
  }



  ArFrame_acquireCamera(m_arSession, m_arFrame, &m_arCamera);

  QMatrix4x4 viewMatrix;
  ArCamera_getViewMatrix(m_arSession, m_arCamera, viewMatrix.data());

  QMatrix4x4 projectionMatrix;
  ArCamera_getProjectionMatrix(m_arSession, m_arCamera, 0.1f, 100.f, projectionMatrix.data());

  m_modelViewProjection = projectionMatrix * viewMatrix;

  ArTrackingState trackingState;
  ArCamera_getTrackingState(m_arSession, m_arCamera, &trackingState);
  //  AR_TRACKING_STATE_TRACKING = 0
  //  AR_TRACKING_STATE_PAUSED = 1
  //  AR_TRACKING_STATE_STOPPED = 2

  int64_t timestamp = 0;
  ArFrame_getTimestamp(m_arSession, m_arFrame, &timestamp); // timestamp in nanoseconds

  ArPose* pose = nullptr;
  ArPose_create(m_arSession, nullptr, &pose);
  ArCamera_getDisplayOrientedPose(m_arSession, m_arCamera, pose);
  ArPose_getPoseRaw(m_arSession, pose, m_pose);
  ArPose_destroy(pose);




  // Update and render point cloud.
  ArStatus point_cloud_status = ArFrame_acquirePointCloud(m_arSession, m_arFrame, &m_arPointCloud);
  if (point_cloud_status == AR_SUCCESS)
  {
    ArPointCloud_getNumberOfPoints(m_arSession, m_arPointCloud, &m_pointCloudSize);
    if (m_pointCloudSize <= 0)
    {
      return;
    }

    ArPointCloud_getData(m_arSession, m_arPointCloud, &m_pointCloudData);
  }

  // TODO: next?
  // Get light estimation value.
  // Render anchored objects.
  // Update and render point cloud.
}

void ArCoreWrapper::afterRendering()
{
  if (m_arPointCloud)
  {
    ArPointCloud_release(m_arPointCloud);
    m_arPointCloud = nullptr;
  }

  if (m_arCamera)
  {
    ArCamera_release(m_arCamera);
    m_arCamera = nullptr;
  }

  if (m_arFrame)
  {
    ArFrame_destroy(m_arFrame);
    m_arFrame = nullptr;
  }
}

void ArCoreWrapper::destroy()
{
  if (m_arSession)
  {
    ArSession_destroy(m_arSession);
    m_arSession = nullptr;
  }
}

//TransformationMatrix ArCoreWrapper::transformationMatrix() const
//{
//  constexpr double tscale = 250; // HorizontalSpeedFactor;
//  return TransformationMatrix(m_pose[0], m_pose[1], m_pose[2], m_pose[3],
//      m_pose[4] * tscale, m_pose[5] * tscale, m_pose[6] * tscale);
//}

float* ArCoreWrapper::transformedUvs() const
{
  return m_transformedUvs;
}

const float* ArCoreWrapper::modelViewProjectionData() const
{
  return m_modelViewProjection.data();
}

const float* ArCoreWrapper::pointCloudData() const
{
  return m_pointCloudData;
}

int32_t ArCoreWrapper::pointCloudSize() const
{
  return m_pointCloudSize;
}
