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

#ifndef ArKitWrapper_H
#define ArKitWrapper_H

//#include <QSize>
#include <QOpenGLFunctions>

#include "TransformationMatrix.h"
#include "ArKitFrameRenderer.h"
#include "ArKitPointCloudRenderer.h"
#include <QOpenGLTexture>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // internal?
{

class ArcGISArView;

// https://developer.apple.com/documentation/arkit/displaying_an_ar_experience_with_metal?language=objc
// https://developer.apple.com/documentation/arkit/arframe/2867984-capturedimage?language=objc
// https://stackoverflow.com/questions/32850012/what-is-the-most-efficient-way-to-display-cvimagebufferref-on-ios
// https://discussions.apple.com/thread/2597309

class ArKitWrapper
{
public:
  ArKitWrapper(ArcGISArView* arcGISArView);
  ~ArKitWrapper();

  bool isValid() const;

  void startTracking();
  void stopTracking();

  // properties
  QSizeF size() const;
  void setSize(const QSizeF& size);

  QMatrix4x4 viewMatrix() const;
  QMatrix4x4 projectionMatrix() const;
  const float* pose() const;

  void init();
  void beforeRendering();
  void afterRendering();
  void render();

  // low level access to AR kit
  // ARSession* arSession() const;

  //private:
  bool install();
  void create();
  void pause();
  void resume();
  void update();
  void destroy();

  TransformationMatrix transformationMatrix() const;
  float* transformedUvs() const;
  float* modelViewProjectionData() const;
  const float* pointCloudData() const;
  int32_t pointCloudSize() const;

private:
  Q_DISABLE_COPY(ArKitWrapper)

  struct ArKitWrapperPrivate;
  ArKitWrapperPrivate* m_impl = nullptr;

  ArcGISArView* m_arcGISArView = nullptr;

  ArKitFrameRenderer m_arKitFrameRenderer;
  ArKitPointCloudRenderer m_arKitPointCloudRenderer;

  void createTextureIds();

  QOpenGLTexture m_textureY;
  QOpenGLTexture m_textureCbCr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArKitWrapper_H
