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

#include "ArcGISArViewInterface.h"
#include "Camera.h"
#include "SceneQuickView.h"

namespace Esri
{
namespace ArcGISRuntime
{

class TransformationMatrixCameraController;

namespace Toolkit
{

// Implementation of the AR scene view for the C++ API

class /*TOOLKIT_EXPORT*/ ArcGISArView : public ArcGISArViewInterface
{
  Q_OBJECT

  Q_PROPERTY(Camera originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  Q_PROPERTY(SceneQuickView* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)

public:
  explicit ArcGISArView(QQuickItem* parent = nullptr);
  explicit ArcGISArView(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent = nullptr); // todo: remove "ArKit" in the name?
  ~ArcGISArView() override;

  // properties
  Camera originCamera() const;
  void setOriginCamera(const Camera& originCamera);

  SceneQuickView* sceneView() const;
  void setSceneView(SceneQuickView* sceneView);

  void setTranslationFactor(double translationFactor) override;

  // methods invokable?
  Q_INVOKABLE Point screenToLocation(const Point& screenPoint) const;

  // update the matrix transformation
  void updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                    double translationX, double translationY, double translationZ) override;

  void updateFieldOfView(double xFocalLength, double yFocalLength,
                         double xPrincipal, double yPrincipal,
                         double xImageSize, double yImageSize) override;

  // update scene view render
  void renderFrame() override;

signals:
  void originCameraChanged();
  void sceneViewChanged();

private:
  int m_timerId = 0;
  Camera m_originCamera;
  SceneQuickView* m_sceneView = nullptr;
  TransformationMatrixCameraController* m_tmcc = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEW_H
