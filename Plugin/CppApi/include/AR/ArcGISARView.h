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

#include "SceneQuickView.h"
#include "Point.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class TOOLKIT_EXPORT ArcGISARView : public SceneQuickView
{
  Q_OBJECT

  // Q_PROPERTY(ARCNView)
  Q_PROPERTY(Camera originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  // Q_PROPERTY(sceneView)
  Q_PROPERTY(double translationTransformationFactor READ translationTransformationFactor WRITE setTranslationTransformationFactor NOTIFY translationTransformationFactorChanged)
  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)

  // Q_PROPERTY(default)

public:
  ArcGISARView(QObject* parent = nullptr);
  ArcGISARView(bool renderVideoFeed, QObject* parent = nullptr); // implicit cast ptr to bool???
  ~ArcGISARView() override;

  // properties
  Camera originCamera() const;
  void setOriginCamera(const Camera& originCamera);

  double translationTransformationFactor() const;
  void setTranslationTransformationFactor(double translationTransformationFactor);

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  // methods
  Q_INVOKABLE Point arScreenToLocation(const Point& screenPoint) const;
  Q_INVOKABLE void resetTracking();
  Q_INVOKABLE bool resetUsingLocationService();
  Q_INVOKABLE void resetUsingSpacialAnchor();
  Q_INVOKABLE void startTracking();
  Q_INVOKABLE void stopTracking();

signals:
  void originCameraChanged();
  void translationTransformationFactorChanged();
  void renderVideoFeedChanged();
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEW_H
