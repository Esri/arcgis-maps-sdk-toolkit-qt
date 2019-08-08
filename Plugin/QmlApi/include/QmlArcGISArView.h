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
namespace Internal {

class /*TOOLKIT_EXPORT*/ QmlArcGISArView : public ArcGISArViewInterface
{
  Q_OBJECT

  Q_PROPERTY(QObject* originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  Q_PROPERTY(QObject* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)

public:
  explicit QmlArcGISArView(QQuickItem* parent = nullptr);
  ~QmlArcGISArView() override;

  // properties
  QObject* originCamera() const;
  void setOriginCamera(QObject* originCamera);

  QObject* sceneView() const;
  void setSceneView(QObject* sceneView);

  // methods invokable?
  Q_INVOKABLE QObject* screenToLocation(QObject* screenPoint) const;

public: // todo: internals?
  // update the matrix transformation
  void updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                    double translationX, double translationY, double translationZ) override;

  void updateFieldOfView(double xFocalLength, double yFocalLength,
                         double xPrincipal, double yPrincipal,
                         double xImageSize, double yImageSize) override;

  void renderFrame() override;

  void setTranslationFactorInternal(double translationFactor) override;

signals:
  void originCameraChanged();
  void sceneViewChanged();

private:
  QObject* createQmlWrapper() const;
  bool assertClassName(QObject* object, const QString& className) const;

  QObject* m_originCamera = nullptr;
  QObject* m_sceneView = nullptr;
  mutable QObject* m_tmcc = nullptr;
  mutable QObject* m_qmlWrapper = nullptr;
};

} // Internal namespace
} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // QmlArcGISArView_H
