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

#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "ArKitFrameRenderer.h"
#include "ArKitPointCloudRenderer.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class ArcGISArViewInterface;

class ArKitWrapper
{
public:
  ArKitWrapper(ArcGISArViewInterface* arcGISArView);
  ~ArKitWrapper();

  void startTracking();
  void stopTracking();
  void resetTracking();
  bool isTracking() const;

  // properties
  void setSize(const QSizeF& size);

  void initGL();
  void beforeRendering();
  void afterRendering();
  void render();

  // low level access to AR kit. Not implemented.
  //  ARSession* session() const;
  //  ARConfiguration* configuration() const;
  //  ArSessionDelegate* delegate() const;

  // point cloud data
  float* modelViewProjectionData() const;
  const float* pointCloudData() const;
  int32_t pointCloudSize() const;

private:
  Q_DISABLE_COPY(ArKitWrapper)

  struct ArKitWrapperPrivate;
  ArKitWrapperPrivate* m_impl = nullptr;

  ArKitFrameRenderer m_arKitFrameRenderer;
  ArKitPointCloudRenderer m_arKitPointCloudRenderer;

  // todo: move that to ArKitFrameRenderer
  QOpenGLTexture m_textureY;
  QOpenGLTexture m_textureCbCr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArKitWrapper_H
