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
#include "LocationDataSource.h"
#include "LocationTrackingMode.h"
#include <array>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

namespace Internal {
class ArWrapper;
class ArcGISArViewRenderer;
}

class ArcGISArViewInterface : public QQuickFramebufferObject
{
  Q_OBJECT
  Q_PROPERTY(bool tracking READ tracking WRITE setTracking NOTIFY trackingChanged)
  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)
  Q_PROPERTY(double translationFactor READ translationFactor WRITE setTranslationFactor NOTIFY translationFactorChanged)
  Q_PROPERTY(bool tryUsingArKit READ tryUsingArKit WRITE setTryUsingArKit NOTIFY tryUsingArKitChanged)

  // sensor
  Q_PROPERTY(LocationDataSource* locationDataSource READ locationDataSource
             WRITE setLocationDataSource NOTIFY locationDataSourceChanged)
  Q_PROPERTY(LocationTrackingMode locationTrackingMode READ locationTrackingMode
             WRITE setLocationTrackingMode NOTIFY locationTrackingModeChanged)

  // properties for debug mode
  Q_PROPERTY(QColor pointCloudColor READ pointCloudColor WRITE setPointCloudColor NOTIFY pointCloudColorChanged)
  Q_PROPERTY(int pointCloudSize READ pointCloudSize WRITE setPointCloudSize NOTIFY pointCloudSizeChanged)
  Q_PROPERTY(QColor planeColor READ planeColor WRITE setPlaneColor NOTIFY planeColorChanged)

protected:
  explicit ArcGISArViewInterface(QQuickItem* parent = nullptr);
  explicit ArcGISArViewInterface(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent = nullptr);

public:
  ~ArcGISArViewInterface() override;

  // properties
  bool tracking() const;
  void setTracking(bool tracking);

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  double translationFactor() const;
  void setTranslationFactor(double translationFactor);

  bool tryUsingArKit() const;
  void setTryUsingArKit(bool tryUsingArKit);

  // sensors
  LocationDataSource* locationDataSource() const;
  void setLocationDataSource(LocationDataSource* locationDataSource);

  LocationTrackingMode locationTrackingMode() const;
  void setLocationTrackingMode(LocationTrackingMode locationTrackingMode);

  // invokable methods
  Q_INVOKABLE void resetTracking();
  Q_INVOKABLE void startTracking(LocationTrackingMode locationTrackingMode = LocationTrackingMode::Ignore);
  Q_INVOKABLE void stopTracking();

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

  QColor planeColor() const;
  void setPlaneColor(const QColor& planeColor);

  // low access to the ARKit/ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;

  // override function from QQuickFramebufferObject
  QQuickFramebufferObject::Renderer* createRenderer() const override;

signals:
  void renderVideoFeedChanged();
  void trackingChanged();
  void translationFactorChanged();
  void tryUsingArKitChanged();

  // error handling
  void errorOccurred(const QString& errorMessage, const QString& additionalMessage);

  // sensor
  void locationDataSourceChanged();
  void locationTrackingModeChanged();

  // properties for debug mode
  void pointCloudColorChanged();
  void pointCloudSizeChanged();
  void planeColorChanged();

public: // internals, used by AR wrappers
  virtual void setTransformationMatrixInternal(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                               double translationX, double translationY, double translationZ) = 0;

  virtual void setFieldOfViewInternal(double xFocalLength, double yFocalLength,
                                      double xPrincipal, double yPrincipal,
                                      double xImageSize, double yImageSize) = 0;

  virtual void renderFrameInternal() = 0;

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

  // internals, used by AR view derived classes
  std::array<double, 7> hitTestInternal(int x, int y) const;

  virtual void setTranslationFactorInternal(double translationFactor) = 0;
  virtual void setLocationInternal(double latitude, double longitude, double altitude) = 0;
  virtual void setHeadingInternal(double heading) = 0;
  virtual void resetTrackingInternal() = 0;

private:
  void updateTrackingSources();

  mutable Internal::ArcGISArViewRenderer* m_arViewRenderer = nullptr;
  std::unique_ptr<Internal::ArWrapper> m_arWrapper;

  bool m_tracking = false;
  bool m_renderVideoFeed = true;
  double m_translationFactor = 1.0;
  bool m_tryUsingArKit = true;

  // sensors
  LocationDataSource* m_locationDataSource = nullptr;
  LocationTrackingMode m_locationTrackingMode = LocationTrackingMode::Ignore;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArcGISArViewInterface_H
