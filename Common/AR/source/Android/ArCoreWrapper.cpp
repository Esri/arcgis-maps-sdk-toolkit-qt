/*******************************************************************************
 *  Copyright 2012-2019 Esri
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
#include "ArCorePlaneRenderer.h"
#include <QGuiApplication>
#include <QScreen>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Wrap the ARCore for Android.
  \sa {AR}
 */

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
  m_arCorePlaneRenderer(new ArCorePlaneRenderer(this)),
  m_arCorePointCloudRenderer(new ArCorePointCloudRenderer(this))
{
  installArCore();
  createArSession();

  // update frames when timer timeout
  QObject::connect(&m_timer, &QTimer::timeout, [this]()
  {
    // request the update of the view (in main thread), to render the AR frame (in GL thread).
    m_arcGISArView->update();

    // update the scene view camera
    auto camera = quaternionTranslation();
    m_arcGISArView->setTransformationMatrixInternal(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6]);

    // udapte the field of view, based on the
    auto lens = lensIntrinsics();
    m_arcGISArView->setFieldOfViewInternal(lens[0], lens[1], lens[2], lens[3], lens[4], lens[5]);

    // render the frame of the ArcGIS runtime
    m_arcGISArView->renderFrameInternal();
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
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to resume the AR session.");
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
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to pause the AR session.");
    return;
  }
}

void ArCoreWrapper::resetTracking()
{
  // todo: arSCNView.session.run(arConfiguration, options: [.resetTracking, .removeExistingAnchors])
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

  // sets the texture id created by OpenGL context
  ArSession_setCameraTextureName(m_arSession, textureId);
}

// this function run on the rendering thread
void ArCoreWrapper::initGL()
{
  m_arCoreFrameRenderer.initGL();

  if (m_arCorePlaneRenderer)
    m_arCorePlaneRenderer->initGL();

  if (m_arCorePointCloudRenderer)
    m_arCorePointCloudRenderer->initGL();
}

// this function run on the rendering thread
void ArCoreWrapper::render()
{
  udpateArCamera();

  m_arCoreFrameRenderer.render();

  if (m_arCorePlaneRenderer)
    m_arCorePlaneRenderer->render();

  if (m_arCorePointCloudRenderer)
    m_arCorePointCloudRenderer->render();
}

// the application's JNIEnv object
JNIEnv* ArCoreWrapper::jniEnvironment()
{
  return m_jniEnvironment;
}

// the jobject referencing the application's current Android Activity.
jobject ArCoreWrapper::applicationActivity()
{
  // note: AR run in different thread, must to create local reference to avoid deleted object.
  if (m_applicationActivity == nullptr)
    m_applicationActivity = jniEnvironment()->NewLocalRef(QtAndroid::androidActivity().object<jobject>());

  return m_applicationActivity;
}

// initiates installation of ARCore if needed. May be called prior to ArSession_create()
bool ArCoreWrapper::installArCore()
{
  if (m_arSession != nullptr)
    return true;

  ArInstallStatus installStatus;
  ArStatus error = ArCoreApk_requestInstall(jniEnvironment(), applicationActivity(), m_installRequested, &installStatus);
  if (error != AR_SUCCESS)
    return false;

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

// create the AR session and needed objects.
void ArCoreWrapper::createArSession()
{
  if (m_arSession)
    return;

  // try to create the ARCore session. This function can fail if the user reject the autorization
  // to install ARCore.
  auto status = ArSession_create(jniEnvironment(), applicationActivity(), &m_arSession);
  if (status != AR_SUCCESS || !m_arSession)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to create the AR session.");
    return;
  }

  // request camera permission
  const QString permissionKey = QStringLiteral("android.permission.CAMERA");
  auto permissions = QtAndroid::requestPermissionsSync({ permissionKey }, 5000);
  if (permissions[permissionKey] != QtAndroid::PermissionResult::Granted)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to access to the camera.");
    return;
  }

  // create the ARCore config.
  ArConfig* arConfig = nullptr;
  ArConfig_create(m_arSession, &arConfig);
  if (arConfig == nullptr)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to create the AR config.");
    return;
  }

  ArConfig_setFocusMode(m_arSession, arConfig, AR_FOCUS_MODE_AUTO);
  status = ArSession_configure(m_arSession, arConfig);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to configure the AR session.");
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
      emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to create an AR frame.");
      return;
    }
  }

  // update the session and received the last available frame.
  ArStatus status = ArSession_update(m_arSession, m_arFrame);
  if (status != AR_SUCCESS)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to update the AR frame.");
    return;
  }

  // If display rotation changed (also includes view size change), we need to
  // re-query the uv coordinates for the on-screen portion of the camera image.
  int32_t geometryChanged = 0;
  ArFrame_getDisplayGeometryChanged(m_arSession, m_arFrame, &geometryChanged);
  if (geometryChanged != 0 || !m_uvsInitialized)
  {
    // get the screen orientation
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
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to acquire the camera.");
    return;
  }

  // get the view and projection matrix
  QMatrix4x4 viewMatrix;
  QMatrix4x4 projectionMatrix;
  ArCamera_getViewMatrix(m_arSession, m_arCamera, viewMatrix.data());
  ArCamera_getProjectionMatrix(m_arSession, m_arCamera, 0.1f, 100.f, projectionMatrix.data());

  // calculate the model-view-projection matrix. (The model matrix is the identity matrix)
  // get the screen orientation
  QMatrix4x4 deviceOrientationMatrix;
  const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

  switch (orientation)
  {
    case Qt::PortraitOrientation:
      // do nothing
      break;

    case Qt::LandscapeOrientation:
      deviceOrientationMatrix.rotate(-90.0, 0.0f, 0.0f, -1.0f);
      break;

    case Qt::InvertedPortraitOrientation:
      // not supported
      break;

    case Qt::InvertedLandscapeOrientation:
      deviceOrientationMatrix.rotate(90.0, 0.0f, 0.0f, -1.0f);
      break;

    default:
      // do nothing
      break;
  }

  m_mvpMatrix = deviceOrientationMatrix * projectionMatrix * viewMatrix;
}

bool ArCoreWrapper::renderVideoFeed() const
{
  return m_renderVideoFeed;
}

void ArCoreWrapper::setRenderVideoFeed(bool renderVideoFeed)
{
  m_renderVideoFeed = renderVideoFeed;
}

// The returned array contains the following parameters: the quaternions x, y, z and w
// end the translations x, y and z. These parameters can be used to create a transformation
// matrix object using the "createWithQuaternionAndTranslation" function.
std::array<double, 7> ArCoreWrapper::quaternionTranslation() const
{
  if (!m_arSession)
    return {};

  ArPose* pose = nullptr;
  ArPose_create(m_arSession, nullptr, &pose);
  if (!pose)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to create an AR pose.");
    return {};
  }

  float poseRaw[7] = {};
  ArCamera_getDisplayOrientedPose(m_arSession, m_arCamera, pose);
  ArPose_getPoseRaw(m_arSession, pose, poseRaw);
  ArPose_destroy(pose);

  // get the screen orientation
  const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

  switch (orientation)
  {
    case Qt::PortraitOrientation:
      return { poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6] };
      break;

    case Qt::LandscapeOrientation:
    {
      QQuaternion rawQuaternion(poseRaw[3], poseRaw[0], poseRaw[1], poseRaw[2]);
      QQuaternion orientationQuaternion = QQuaternion::fromAxisAndAngle(0, 0, 1, -90);
      auto finalQuaternion = rawQuaternion * orientationQuaternion;

      return { finalQuaternion.x(), finalQuaternion.y(), finalQuaternion.z(), finalQuaternion.scalar(),
            poseRaw[4], poseRaw[5], poseRaw[6] };
      break;
    }

    case Qt::InvertedPortraitOrientation:
      // not supported
      break;

    case Qt::InvertedLandscapeOrientation:
    {
      QQuaternion rawQuaternion(poseRaw[3], poseRaw[0], poseRaw[1], poseRaw[2]);
      QQuaternion orientationQuaternion = QQuaternion::fromAxisAndAngle(0, 0, 1, 90);
      auto finalQuaternion = rawQuaternion * orientationQuaternion;

      return { finalQuaternion.x(), finalQuaternion.y(), finalQuaternion.z(), finalQuaternion.scalar(),
            poseRaw[4], poseRaw[5], poseRaw[6] };
      break;
    }

    default:
      return { poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6] };
      break;
  }

  return { poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6] };
}

// The returned array contains the following parameters: xFocalLength, yFocalLength,
// xPrincipal, yPrincipal, xImageSize and yImageSize.
// These parameters can be used to set the field of view in the scene view with the
// "setFieldOfViewFromLensIntrinsics" function.
std::array<double, 6> ArCoreWrapper::lensIntrinsics() const
{
  if (!m_arSession)
    return {};

  ArCameraIntrinsics* cameraIntrinsics = nullptr;
  ArCameraIntrinsics_create(m_arSession, &cameraIntrinsics);
  if (!cameraIntrinsics)
  {
    emit m_arcGISArView->errorOccurred("ARCore failure", "Fails to create an AR camera instrinsics.");
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

  return
  {
    static_cast<double>(xFocalLength), static_cast<double>(yFocalLength),
    static_cast<double>(xPrincipal), static_cast<double>(yPrincipal),
    static_cast<double>(xImageSize), static_cast<double>(yImageSize)
  };
}

// doc: https://developers.google.com/ar/reference/c/group/frame#arframe_hittest
std::array<double, 7> ArCoreWrapper::hitTest(int x, int y) const
{
  if (!m_arSession || !m_arCamera)
    return {};

  // create hit result list
  ArHitResultList* hitResults = nullptr;
  ArHitResultList_create(m_arSession, &hitResults);
  if (!hitResults)
    return {};

  // try to find the location point
  ArFrame_hitTest(m_arSession, m_arFrame, x, y, hitResults);

  int32_t size = 0;
  ArHitResultList_getSize(m_arSession, hitResults, &size);
  if (size <= 0)
  {
    ArHitResultList_destroy(hitResults);
    return {};
  }

  ArHitResult* hitResult = nullptr;
  ArHitResult_create(m_arSession, &hitResult);
  if (!hitResult)
  {
    ArHitResultList_destroy(hitResults);
    return {};
  }

  ArHitResultList_getItem(m_arSession, hitResults, 0, hitResult);

  ArPose* pose = nullptr;
  ArPose_create(m_arSession, nullptr, &pose);
  if (!hitResult)
  {
    ArHitResult_destroy(hitResult);
    ArHitResultList_destroy(hitResults);
    return {};
  }

  ArHitResult_getHitPose(m_arSession, hitResult, pose);

  float poseRaw[7] = {};
  ArPose_getPoseRaw(m_arSession, pose, poseRaw);

  // destroys objects
  ArPose_destroy(pose);
  ArHitResult_destroy(hitResult);
  ArHitResultList_destroy(hitResults);

  return { poseRaw[0], poseRaw[1], poseRaw[2], poseRaw[3], poseRaw[4], poseRaw[5], poseRaw[6] };
}

const float* ArCoreWrapper::transformedUvs() const
{
  return m_transformedUvs;
}

// properties for debug mode
QColor ArCoreWrapper::pointCloudColor() const
{
  if (m_arCorePointCloudRenderer)
    return m_arCorePointCloudRenderer->pointCloudColor();
  return QColor();
}

void ArCoreWrapper::setPointCloudColor(const QColor& pointCloudColor)
{
  if (pointCloudColor.isValid())
  {
    if (!m_arCorePointCloudRenderer)
    {
      m_arCorePointCloudRenderer.reset(new ArCorePointCloudRenderer(this));
      m_arCorePointCloudRenderer->initGL();
    }

    m_arCorePointCloudRenderer->setPointCloudColor(pointCloudColor);
  }
  else
  {
    m_arCorePointCloudRenderer.reset();
  }
}

int ArCoreWrapper::pointCloudSize() const
{
  if (m_arCorePointCloudRenderer)
    return m_arCorePointCloudRenderer->pointCloudSize();
  return -1;
}

void ArCoreWrapper::setPointCloudSize(int pointCloudSize)
{
  if (pointCloudSize > 0)
  {
    if (!m_arCorePointCloudRenderer)
    {
      m_arCorePointCloudRenderer.reset(new ArCorePointCloudRenderer(this));
      m_arCorePointCloudRenderer->initGL();
    }

    m_arCorePointCloudRenderer->setPointCloudSize(pointCloudSize);
  }
  else
  {
    m_arCorePointCloudRenderer.reset();
  }
}

QColor ArCoreWrapper::planeColor() const
{
  if (m_arCorePlaneRenderer)
    return m_arCorePlaneRenderer->planeColor();
  return QColor();
}

void ArCoreWrapper::setPlaneColor(const QColor& planeColor)
{
  if (planeColor.isValid())
  {
    if (!m_arCorePlaneRenderer)
    {
      m_arCorePlaneRenderer.reset(new ArCorePlaneRenderer(this));
      m_arCorePlaneRenderer->initGL();
    }

    m_arCorePlaneRenderer->setPlaneColor(planeColor);
  }
  else
  {
    m_arCorePlaneRenderer.reset();
  }
}

// Methods for point cloud data.
// Be careful with the concurrent access and lifetime: the data are initialized and
// released in the main thread and used in the GL thread.
void ArCoreWrapper::planeListData(int32_t& size)
{
  if (!m_arSession || m_arPlaneList)
    return;

  ArTrackableList_create(m_arSession, &m_arPlaneList);
  if (m_arPlaneList == nullptr)
    return;

  ArTrackableType planeTrackedType = AR_TRACKABLE_PLANE;
  ArSession_getAllTrackables(m_arSession, planeTrackedType, m_arPlaneList);

  ArTrackableList_getSize(m_arSession, m_arPlaneList, &size);
}

bool ArCoreWrapper::planeData(QMatrix4x4& mvp, int32_t index, std::vector<float>& vertices)
{
  ArTrackableList_acquireItem(m_arSession, m_arPlaneList, index, &m_arTrackable);
  ArPlane* arPlane = ArAsPlane(m_arTrackable);

  ArTrackingState outTrackingState = AR_TRACKING_STATE_STOPPED;
  ArTrackable_getTrackingState(m_arSession, m_arTrackable, &outTrackingState);

  ArPlane* subsumePlane = nullptr;
  ArPlane_acquireSubsumedBy(m_arSession, arPlane, &subsumePlane);
  if (subsumePlane != nullptr)
  {
    ArTrackable_release(ArAsTrackable(subsumePlane));
    return false;
  }

  if (outTrackingState != AR_TRACKING_STATE_TRACKING)
    return false;

  ArTrackingState planeTrackingState = AR_TRACKING_STATE_STOPPED;
  ArTrackable_getTrackingState(m_arSession, ArAsTrackable(arPlane), &planeTrackingState);

  if (planeTrackingState != AR_TRACKING_STATE_TRACKING)
    return false;

  int32_t polygonLength = 0;
  ArPlane_getPolygonSize(m_arSession, arPlane, &polygonLength);

  if (polygonLength == 0)
  {
    qDebug() << "PlaneRenderer::UpdatePlane, no valid plane polygon is found";
    return false;
  }

  vertices.resize(polygonLength);
  ArPlane_getPolygon(m_arSession, arPlane, vertices.data());

  ArPose* arPlanePose = nullptr;
  ArPose_create(m_arSession, nullptr, &arPlanePose);
  ArPlane_getCenterPose(m_arSession, arPlane, arPlanePose);

  QMatrix4x4 model;
  ArPose_getMatrix(m_arSession, arPlanePose, model.data());
  mvp = m_mvpMatrix * model;

  return true;
}

void ArCoreWrapper::releasePlaneData()
{
  if (!m_arTrackable)
    return;

  ArTrackable_release(m_arTrackable);
  m_arTrackable = nullptr;
}

void ArCoreWrapper::releasePlaneListData()
{
  if (!m_arPlaneList)
    return;

  ArTrackableList_destroy(m_arPlaneList);
  m_arPlaneList = nullptr;
}

// Methods for point cloud data.
// Be careful with the concurrent access and lifetime: the data are initialized and
// released in the main thread and used in the GL thread.
void ArCoreWrapper::pointCloudData(QMatrix4x4& mvp, int32_t& size, const float** data) // todo: change to std::vector<float>
{
  if (!m_arSession || !m_arCorePointCloudRenderer || m_arPointCloud)
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

  // gets the positions of the could points
  ArPointCloud_getData(m_arSession, m_arPointCloud, data);

  // calculate the model-view-projection matrix. (The model matrix is the identity matrix)
  mvp = m_mvpMatrix;
}

// The point cloud data can be released after the rendering is done.
void ArCoreWrapper::releasePointCouldData()
{
  if (!m_arPointCloud)
    return;

  ArPointCloud_release(m_arPointCloud);
  m_arPointCloud = nullptr;
}

/*!
 * \internal
 * Low access to the ARCore objects.
 */
template<>
ArSession* ArCoreWrapper::arRawPtr<ArSession>() const
{
  return m_arSession;
}

/*!
 * \internal
 * Low access to the ARCore objects.
 */
template<>
ArFrame* ArCoreWrapper::arRawPtr<ArFrame>() const
{
  return m_arFrame;
}

/*!
 * \internal
 * Low access to the ARCore objects.
 */
template<>
ArCamera* ArCoreWrapper::arRawPtr<ArCamera>() const
{
  return m_arCamera;
}
