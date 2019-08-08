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

#ifndef ArcGISArViewInterface_H
#define ArcGISArViewInterface_H

#include <QQuickFramebufferObject>
#include <array>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

namespace Internal {
class ArWrapper;
class ArcGISArViewRenderer;
}

class LocationDataSource;
class OrientationDataSource;

class ArcGISArViewInterface : public QQuickFramebufferObject
{
  Q_OBJECT
  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)
  Q_PROPERTY(bool tracking READ tracking WRITE setTracking NOTIFY trackingChanged)
  Q_PROPERTY(double translationFactor READ translationFactor WRITE setTranslationFactor NOTIFY translationFactorChanged)
  Q_PROPERTY(bool tryUsingArKit READ tryUsingArKit WRITE setTryUsingArKit NOTIFY tryUsingArKitChanged)

  // sensors
  Q_PROPERTY(LocationDataSource* locationDataSource READ locationDataSource WRITE setLocationDataSource NOTIFY locationDataSourceChanged)
  Q_PROPERTY(OrientationDataSource* orientationDataSource READ orientationDataSource WRITE setOrientationDataSource NOTIFY orientationDataSourceChanged)

public:
  explicit ArcGISArViewInterface(QQuickItem* parent = nullptr);
  explicit ArcGISArViewInterface(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent = nullptr);
  ~ArcGISArViewInterface() override;

  // properties
  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  bool tracking() const;
  void setTracking(bool tracking);

  double translationFactor() const;
  void setTranslationFactor(double translationFactor);

  bool tryUsingArKit() const;
  void setTryUsingArKit(bool tryUsingArKit);

  // sensors
  LocationDataSource* locationDataSource() const;
  void setLocationDataSource(LocationDataSource* locationDataSource);

  OrientationDataSource* orientationDataSource() const;
  void setOrientationDataSource(OrientationDataSource* orientationDataSource);

  // invokable methods
  Q_INVOKABLE void resetTracking();
  Q_INVOKABLE bool resetUsingLocationService();
  Q_INVOKABLE void resetUsingSpacialAnchor();
  Q_INVOKABLE void startTracking();
  Q_INVOKABLE void stopTracking();

  // low access to the ARKit/ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

  // internals functions
  QQuickFramebufferObject::Renderer* createRenderer() const override;

  virtual void updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                            double translationX, double translationY, double translationZ) = 0;

  virtual void updateFieldOfView(double xFocalLength, double yFocalLength,
                                 double xPrincipal, double yPrincipal,
                                 double xImageSize, double yImageSize) = 0;

  virtual void renderFrame() = 0;

  virtual void setTranslationFactorInternal(double translationFactor) = 0;

signals:
  void renderVideoFeedChanged();
  void trackingChanged();
  void translationFactorChanged();
  void tryUsingArKitChanged();

  void errorOccurred(const QString& errorMessage, const QString& additionalMessage);

  // sensors
  void locationDataSourceChanged();
  void orientationDataSourceChanged();

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

  std::array<double, 7> screenToLocationInternal(int x, int y) const;

private:
  void updateTrackingSources();

  mutable Internal::ArcGISArViewRenderer* m_arViewRenderer = nullptr;
  std::unique_ptr<Internal::ArWrapper> m_arWrapper;

  bool m_renderVideoFeed = true;
  double m_translationFactor = 1.0;
  bool m_tryUsingArKit = true;
  bool m_tracking = true;

  // sensors
  LocationDataSource* m_locationDataSource = nullptr;
  OrientationDataSource* m_orientationDataSource = nullptr;
  QMetaObject::Connection m_locationDataSourceConnection;
  QMetaObject::Connection m_orientationDataSourceConnection;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArcGISArViewInterface_H
