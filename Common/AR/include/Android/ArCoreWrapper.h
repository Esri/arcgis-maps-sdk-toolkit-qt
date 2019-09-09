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

#ifndef ARCOREWRAPPER_H
#define ARCOREWRAPPER_H

#include <QAndroidJniEnvironment>
#include <QSize>
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "ArCoreFrameRenderer.h"

// forward declaration of AR core types to avoid include "arcore_c_api.h" here.
typedef struct ArSession_ ArSession;
typedef struct ArFrame_ ArFrame;
typedef struct ArCamera_ ArCamera;
typedef struct ArTrackableList_ ArTrackableList;
typedef struct ArTrackable_ ArTrackable;
typedef struct ArPointCloud_ ArPointCloud;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class ArcGISArViewInterface;
class ArCorePointCloudRenderer;
class ArCorePlaneRenderer;

// todo: add tracking state?
// https://developers.google.com/ar/reference/java/arcore/reference/com/google/ar/core/TrackingFailureReason
// https://developers.google.com/ar/reference/java/arcore/reference/com/google/ar/core/TrackingState
// https://developers.google.com/ar/reference/unity/namespace/GoogleARCore
// - ApkAvailabilityStatus, ApkInstallationStatus, LostTrackingReason, SessionStatus, TrackingState

class ArCoreWrapper
{
public:
  ArCoreWrapper(ArcGISArViewInterface* arcGISArView);
  ~ArCoreWrapper();

  void startTracking();
  void stopTracking();
  void resetTracking();

  // properties
  void setSize(const QSize& size);

  void setTextureId(GLuint textureId);

  void initGL();
  void render();

  void udpateArCamera();
  void releaseArData();

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  // hit test for screen to location feature
  std::array<double, 7> hitTest(int x, int y) const;

  // methods for AR frame rendering
  const float* transformedUvs() const;

  // methods for plane data
  void planeListData(int32_t& size);
  bool planeData(QMatrix4x4& mvp, int32_t index, std::vector<float>& vertices);
  void releasePlaneData();
  void releasePlaneListData();

  // methods for point cloud data
  void pointCloudData(QMatrix4x4& mvp, int32_t& size, const float** data);
  void releasePointCouldData();

  // low access to the ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

private:
  Q_DISABLE_COPY(ArCoreWrapper)

  //private:
  JNIEnv* jniEnvironment();
  jobject applicationActivity();

  bool installArCore();
  void createArSession();

  bool m_renderVideoFeed = true;

  std::array<double, 7> quaternionTranslation() const;
  std::array<double, 6> lensIntrinsics() const;

  ArcGISArViewInterface* m_arcGISArView = nullptr;

  QAndroidJniEnvironment m_jniEnvironment;

  jobject m_applicationActivity = nullptr;

  ArCoreFrameRenderer m_arCoreFrameRenderer;
  std::unique_ptr<ArCorePlaneRenderer> m_arCorePlaneRenderer;
  std::unique_ptr<ArCorePointCloudRenderer> m_arCorePointCloudRenderer;

  void renderArFrame();
  void renderArPlane();
  void renderArPointCloud();

  // When your apllication launches or enters an AR mode,
  // it should call this method with user_requested_install = 1.
  static int32_t m_installRequested;

  GLuint m_textureId = 0;
  bool m_uvsInitialized = false;
  static constexpr int kNumVertices = 4;

  // AR core types
  ArSession* m_arSession = nullptr;
  ArFrame* m_arFrame = nullptr;
  ArCamera* m_arCamera = nullptr;

  // data returned from each frame
  float m_transformedUvs[8] = {};

  QTimer m_timer;

  bool m_renderPlane = true;
  bool m_renderPointCloud = true;
//  bool m_debugOpenGL = true;
//  bool m_debugPerformances = true;

  // attribute for plane and point cloud
  ArTrackableList* m_arPlaneList = nullptr;
  ArTrackable* m_arTrackable = nullptr;
  ArPointCloud* m_arPointCloud = nullptr;

  // the model-view-projection matrix
  QMatrix4x4 m_mvpMatrix;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCOREWRAPPER_H
