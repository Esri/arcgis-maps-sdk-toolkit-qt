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

#ifndef QmlArcGISArView_H
#define QmlArcGISArView_H

#include "ArcGISArViewInterface.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class QmlArcGISArView : public ArcGISArViewInterface
{
  Q_OBJECT

  Q_PROPERTY(QObject* transformationMatrixCameraControler READ transformationMatrixCameraControler
             WRITE setTransformationMatrixCameraControler NOTIFY transformationMatrixCameraControlerChanged)
  Q_PROPERTY(QObject* originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  Q_PROPERTY(QObject* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)

  // todo: add default/qmlObjects for camera, sceneView, etc.

public:
  explicit QmlArcGISArView(QQuickItem* parent = nullptr);
  ~QmlArcGISArView() override;

  // properties
  QObject* originCamera() const;
  void setOriginCamera(QObject* originCamera);

  QObject* sceneView() const;
  void setSceneView(QObject* sceneView);

  QObject* transformationMatrixCameraControler() const;
  void setTransformationMatrixCameraControler(QObject* tmcc);

  // methods invokable
  Q_INVOKABLE QObject* screenToLocation(QObject* screenPoint) const;

signals:
  void transformationMatrixCameraControlerChanged();
  void originCameraChanged();
  void sceneViewChanged();
  void renderFrame();
  void locationChanged(double latitude, double longitude, double altitude);
  void transformationMatrixChanged(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                   double translationX, double translationY, double translationZ);
  void fieldOfViewChanged(double xFocalLength, double yFocalLength,
                          double xPrincipal, double yPrincipal,
                          double xImageSize, double yImageSize);

public: // internals, used by AR wrappers
  void setTransformationMatrixInternal(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                       double translationX, double translationY, double translationZ) override;

  void setFieldOfViewInternal(double xFocalLength, double yFocalLength,
                              double xPrincipal, double yPrincipal,
                              double xImageSize, double yImageSize) override;

  void renderFrameInternal() override;

protected:
  void setTranslationFactorInternal(double translationFactor) override;
  void setLocationInternal(double latitude, double longitude, double altitude) override;
  void resetTrackingInternal() override;

private:
//  QObject* qmlWrapper() const;
//  QObject* tmcc() const;
  bool assertClassName(QObject* object, const QString& className) const;

  QObject* m_originCamera = nullptr;
  QObject* m_sceneView = nullptr;
  mutable QObject* m_tmcc = nullptr;
  mutable QObject* m_qmlWrapper = nullptr;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // QmlArcGISArView_H
