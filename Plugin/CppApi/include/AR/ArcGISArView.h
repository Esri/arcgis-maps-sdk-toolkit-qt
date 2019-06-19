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

#ifndef ARCGISARVIEW_H
#define ARCGISARVIEW_H

#include <QQuickFramebufferObject>
#include "Camera.h"
#include "SceneQuickView.h"
#include "ArWrapper.h"
#include "ArcGISArViewRenderer.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class /*TOOLKIT_EXPORT*/ ArcGISArView : public QQuickFramebufferObject
{
  Q_OBJECT

  Q_PROPERTY(Camera originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  Q_PROPERTY(SceneQuickView* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)
  Q_PROPERTY(double translationTransformationFactor READ translationTransformationFactor
             WRITE setTranslationTransformationFactor NOTIFY translationTransformationFactorChanged)
  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)

  // add to the design?
  Q_PROPERTY(bool tracking READ tracking WRITE setTracking NOTIFY trackingChanged)

  // Q_PROPERTY(default)

public:
  explicit ArcGISArView(QQuickItem* parent = nullptr);
  explicit ArcGISArView(int renderVideoFeed, QQuickItem* parent = nullptr); // implicit cast ptr to bool???
  ~ArcGISArView() override;

  // properties
  Camera originCamera() const;
  void setOriginCamera(const Camera& originCamera);

  SceneQuickView* sceneView() const;
  void setSceneView(SceneQuickView* sceneView);

  double translationTransformationFactor() const;
  void setTranslationTransformationFactor(double translationTransformationFactor);

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  bool tracking() const;
  void setTracking(bool tracking);

  // methods invokable?
  Q_INVOKABLE Point arScreenToLocation(const Point& screenPoint) const;
  Q_INVOKABLE void resetTracking();
  Q_INVOKABLE bool resetUsingLocationService();
  Q_INVOKABLE void resetUsingSpacialAnchor();
  Q_INVOKABLE void startTracking();
  Q_INVOKABLE void stopTracking();

  // low access to the ARKit/ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* getAR() const;

  // create renderer for frame buffer object
  QQuickFramebufferObject::Renderer* createRenderer() const override;

  // update the matrix transformation
  void updateCamera();

signals:
  void originCameraChanged();
  void sceneViewChanged();
  void translationTransformationFactorChanged();
  void renderVideoFeedChanged();
  void trackingChanged();

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
  void timerEvent(QTimerEvent* event) override;

private:
  int m_timerId = 0;
  Camera m_originCamera;

  mutable ArcGISArViewRenderer* m_arViewRenderer = nullptr;
  std::unique_ptr<ArWrapper> m_arWrapper;
  SceneQuickView* m_sceneView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEW_H
