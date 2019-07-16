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

#ifndef ARCOREWRAPPER_H
#define ARCOREWRAPPER_H

#include <QAndroidJniEnvironment>
#include <QSize>
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "ArCoreFrameRenderer.h"
#include "ArCorePointCloudRenderer.h"

// forward declaration of AR core types to avoid include "arcore_c_api.h" here.
typedef struct ArSession_ ArSession;
typedef struct ArFrame_ ArFrame;
typedef struct ArCamera_ ArCamera;
typedef struct ArPointCloud_ ArPointCloud;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class ArcGISArViewInterface;

class ArCoreWrapper
{
public:
  ArCoreWrapper(ArcGISArViewInterface* arcGISArView);
  ~ArCoreWrapper();

  void startTracking();
  void stopTracking();

  // properties
  void setSize(const QSize& size);

  void setTextureId(GLuint textureId);

  void initGL();
  void beforeRendering();
  void render();
  void afterRendering();

  // parameters to render the point cloud
  const float* transformedUvs() const;
  const float* modelViewProjectionData() const;
  const float* pointCloudData() const;
  int32_t pointCloudSize() const;

  // low level access to AR core
  ArSession* session();
  ArFrame* frame();

private:
  //private:
  JNIEnv* jniEnvironment();
  jobject applicationActivity();

  bool installArCore();
  void createArSession();

  std::array<double, 7> lastQuaternionTranslation() const;
  std::array<double, 6> lastLensIntrinsics() const;

  ArcGISArViewInterface* m_arcGISArView = nullptr;

  QAndroidJniEnvironment m_jniEnvironment;

  jobject m_applicationActivity = nullptr;

  ArCoreFrameRenderer m_arCoreFrameRenderer;
  ArCorePointCloudRenderer m_arCorePointCloudRenderer;

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
  ArPointCloud* m_arPointCloud = nullptr;

  // data returned from each frame
  float m_transformedUvs[8] = {};
  QMatrix4x4 m_modelViewProjection;
  const float* m_pointCloudData = nullptr;
  int32_t m_pointCloudSize = 0;

  QTimer m_timer;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCOREWRAPPER_H
