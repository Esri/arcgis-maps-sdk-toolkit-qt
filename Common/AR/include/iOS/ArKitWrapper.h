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

#ifndef ArKitWrapper_H
#define ArKitWrapper_H

#include <QtGlobal>
#include <QSizeF>
#include <QMatrix4x4>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class ArcGISArViewInterface;

namespace Internal {

class ArKitFrameRenderer;
class ArKitPlaneRenderer;
class ArKitPointCloudRenderer;

class ArKitWrapper
{
public:
  ArKitWrapper(ArcGISArViewInterface* arcGISArView);
  ~ArKitWrapper();

  void startTracking();
  void stopTracking();
  void resetTracking();
  bool isTracking() const;

  void setSize(const QSizeF& size);

  void initGL();
  void updateTextures();
  void render();

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

  QColor planeColor() const;
  void setPlaneColor(const QColor& planeColor);

  // hit test for screen to location feature
  std::array<double, 7> hitTest(int x, int y) const;

  // low access to the ARKit objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

  // point cloud data
  QMatrix4x4 viewProjectionMatrix() const;
  std::vector<float> pointCloudData() const;

private:
  Q_DISABLE_COPY(ArKitWrapper)

  bool m_renderVideoFeed = true;

  struct ArKitWrapperPrivate;
  std::unique_ptr<ArKitWrapperPrivate> m_impl;
  std::unique_ptr<ArKitFrameRenderer> m_arKitFrameRenderer;
  std::unique_ptr<ArKitPlaneRenderer> m_arKitPlaneRenderer;
  std::unique_ptr<ArKitPointCloudRenderer> m_arKitPointCloudRenderer;

  QSizeF m_screenSize;
};

} // Internal namespace
} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArKitWrapper_H
