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

#include "TransformationMatrix.h"
#include <QAndroidJniEnvironment>
#include <QSize>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "ArCoreFrameRenderer.h"
#include "ArCorePointCloudRenderer.h"

//// for BackgroundRenderer
////#include <GLES2/gl2.h>
////#include <GLES2/gl2ext.h>
//#include <QOpenGLFunctions>
//#include <QOpenGLShaderProgram>
//#include <QQuickFramebufferObject>
//#include <QPointer>
//#include <vector>

// forward declaration of AR core types to avoid include "arcore_c_api.h" here.
typedef struct ArSession_ ArSession;
typedef struct ArFrame_ ArFrame;
typedef struct ArCamera_ ArCamera;
typedef struct ArPointCloud_ ArPointCloud;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // internal?
{

class ArcGISArView;

class ArCoreWrapper
{
public:
  ArCoreWrapper(ArcGISArView* arView);
  ~ArCoreWrapper();

  void startTracking();
  void stopTracking();

  // properties
  void setSize(const QSize& size);

  void setTextureId(GLuint textureId);

  void init();
  void render();

  // low level access to AR core
  ArSession* session();
  ArFrame* frame();

  //private:
  JNIEnv* jniEnvironment();
  jobject applicationActivity();

  bool install();
  void create();
  void pause();
  void resume();
  void beforeRendering();
  void afterRendering();
  void destroy();

  TransformationMatrix transformationMatrix() const;
  float* transformedUvs() const;
  const float* modelViewProjectionData() const;
  const float* pointCloudData() const;
  int32_t pointCloudSize() const;

private:
  QAndroidJniEnvironment m_jniEnvironment;

  jobject m_applicationActivity = nullptr;

  ArCoreFrameRenderer m_arCoreFrameRenderer;
  ArCorePointCloudRenderer m_arCorePointCloudRenderer;

  // When your apllication launches or enters an AR mode,
  // it should call this method with user_requested_install = 1.
  static int32_t m_installRequested;

  GLuint m_textureId = 0;
  bool uvs_initialized_ = false;
  static constexpr int kNumVertices = 4;

  // AR core types
  ArSession* m_arSession = nullptr; // unique_ptr with custom deleter?
  ArFrame* m_arFrame = nullptr;
  ArCamera* m_arCamera = nullptr;
  ArPointCloud* m_arPointCloud = nullptr;

  // data returned from each frame
  float* m_transformedUvs = nullptr;
  QMatrix4x4 m_modelViewProjection;
  const float* m_pointCloudData = nullptr;
  int32_t m_pointCloudSize = 0;
  float m_pose[7];
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCOREWRAPPER_H
