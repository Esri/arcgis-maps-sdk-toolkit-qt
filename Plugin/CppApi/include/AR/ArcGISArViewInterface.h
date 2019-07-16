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

#ifndef ARCGISARVIEWINTERFACE_H
#define ARCGISARVIEWINTERFACE_H

#include <QQuickFramebufferObject>
#include "ArWrapper.h"
#include "ArcGISArViewRenderer.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class /*TOOLKIT_EXPORT*/ ArcGISArViewInterface : public QQuickFramebufferObject
{
  Q_OBJECT
  Q_PROPERTY(double translationFactor READ translationFactor WRITE setTranslationFactor NOTIFY translationFactorChanged)

  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)
  Q_PROPERTY(bool tryUsingArKit READ tryUsingArKit WRITE setTryUsingArKit NOTIFY tryUsingArKitChanged)

  // add to the design?
  Q_PROPERTY(bool tracking READ tracking WRITE setTracking NOTIFY trackingChanged)

public:
  explicit ArcGISArViewInterface(QQuickItem* parent = nullptr);
  explicit ArcGISArViewInterface(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent = nullptr);
  ~ArcGISArViewInterface() override;

  // properties
  double translationFactor() const;
  virtual void setTranslationFactor(double translationFactor);

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  bool tryUsingArKit() const;
  void setTryUsingArKit(bool tryUsingArKit);

  bool tracking() const;
  void setTracking(bool tracking);

  // methods
  Q_INVOKABLE void resetTracking();
  Q_INVOKABLE bool resetUsingLocationService();
  Q_INVOKABLE void resetUsingSpacialAnchor();
  Q_INVOKABLE void startTracking();
  Q_INVOKABLE void stopTracking();

  // low access to the ARKit/ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

  // create renderer for frame buffer object
  QQuickFramebufferObject::Renderer* createRenderer() const override;

  // update the the renders
  virtual void updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                            double translationX, double translationY, double translationZ) = 0;

  virtual void updateFieldOfView(double xFocalLength, double yFocalLength,
                                 double xPrincipal, double yPrincipal,
                                 double xImageSize, double yImageSize) = 0;

  virtual void renderFrame() = 0;

signals:
  void translationFactorChanged();
  void renderVideoFeedChanged();
  void tryUsingArKitChanged();
  void trackingChanged();

  void errorOccurred(const QString& errorMessage);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
  mutable ArcGISArViewRenderer* m_arViewRenderer = nullptr;
  std::unique_ptr<ArWrapper> m_arWrapper;

  double m_translationFactor = 0.0;
  bool m_renderVideoFeed = true;
  bool m_tryUsingArKit = true;
  bool m_tracking = true;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEWINTERFACE_H
