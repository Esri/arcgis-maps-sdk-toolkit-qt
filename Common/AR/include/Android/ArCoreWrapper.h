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

#ifndef ArCoreWrapper_H
#define ArCoreWrapper_H

#include <QAndroidJniEnvironment>
#include <QSize>
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <array>

// forward declaration of AR core types to avoid include "arcore_c_api.h" here.
using ArSession = struct ArSession_;
using ArFrame = struct ArFrame_;
using ArCamera = struct ArCamera_;
using ArTrackableList = struct ArTrackableList_;
using ArTrackable = struct ArTrackable_;
using ArPointCloud = struct ArPointCloud_;

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class ArcGISArViewInterface;

namespace Internal {

class ArCoreFrameRenderer;
class ArCorePointCloudRenderer;
class ArCorePlaneRenderer;

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
  std::array<float, 8> transformedUvs() const;

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

  QColor planeColor() const;
  void setPlaneColor(const QColor& planeColor);

  // methods for plane data
  void planeListData(int32_t& size);
  void releasePlaneListData();

  bool planeData(int32_t index, QMatrix4x4& mvp, std::vector<float>& vertices);
  void releasePlaneData();

  // methods for point cloud data
  void pointCloudData(QMatrix4x4& mvp, std::vector<float>& pointCloud);
  void releasePointCloudData();

  // low access to the ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

private:
  Q_DISABLE_COPY(ArCoreWrapper)

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

  std::unique_ptr<ArCoreFrameRenderer> m_arCoreFrameRenderer;
  std::unique_ptr<ArCorePlaneRenderer> m_arCorePlaneRenderer;
  std::unique_ptr<ArCorePointCloudRenderer> m_arCorePointCloudRenderer;

  void renderArFrame();
  void renderArPlane();
  void renderArPointCloud();

  // When the application launches or enters an AR mode,
  // it should call this method with user_requested_install = 1.
  static int32_t m_installRequested;

  // Keep the session status.
  bool m_sessionIsPaused = true;

  // Texture id used to render the frames.
  GLuint m_textureId = 0;

  bool m_uvsInitialized = false;
  static constexpr int kNumVertices = 4;

  // AR core types
  ArSession* m_arSession = nullptr;
  ArFrame* m_arFrame = nullptr;
  ArCamera* m_arCamera = nullptr;

  // data returned from each frame
  std::array<float, 8> m_transformedUvs = {};

  // timer to refresh the frames
  QTimer m_timer;

  // attribute for plane and point cloud
  ArTrackableList* m_arPlaneList = nullptr;
  ArTrackable* m_arTrackable = nullptr;
  ArPointCloud* m_arPointCloud = nullptr;

  // the model-view-projection matrix
  QMatrix4x4 m_mvpMatrix;
};

} // Internal namespace
} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArCoreWrapper_H
