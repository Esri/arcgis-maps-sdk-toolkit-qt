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

#include "ToolkitCommon.h"
#include <QObject>
#include "SceneQuickView.h"
#include "Point.h"

#ifdef Q_OS_IOS
class ARSCNView;
#elif defined Q_OS_ANDROID
typedef struct ArSession_ ArSession;
#endif

class ArWrapper;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class TOOLKIT_EXPORT ArcGISARView : public QObject
{
  Q_OBJECT

  // Q_PROPERTY(ARCNView) // Q_PROPERTY with #ifdef?
  Q_PROPERTY(Camera originCamera READ originCamera WRITE setOriginCamera NOTIFY originCameraChanged)
  Q_PROPERTY(SceneQuickView* sceneView READ sceneView NOTIFY sceneViewChanged) // read only?
  // Q_PROPERTY(SceneQuickView* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged) // read-write?
  Q_PROPERTY(double translationTransformationFactor READ translationTransformationFactor WRITE setTranslationTransformationFactor NOTIFY translationTransformationFactorChanged)
  Q_PROPERTY(bool renderVideoFeed READ renderVideoFeed WRITE setRenderVideoFeed NOTIFY renderVideoFeedChanged)

  // Q_PROPERTY(default)

public:
  explicit ArcGISARView(QObject* parent = nullptr);
  explicit ArcGISARView(bool renderVideoFeed, QObject* parent = nullptr); // implicit cast ptr to bool???
  ~ArcGISARView() override;

  // get the ARKit/ARCore object
  // 1. get raw ptr. Need static cast to use it
  // void* getARsceneVoid() const;
  // 2. get using template. Compile-time error if T is invalid, then can return a reference.
  // template<typename T>
  // T& getARscene() const;
  // 3. plateform specific code
#ifdef Q_OS_IOS
public:
  ARSCNView* getARscene() const;
private:
  ARSCNView* internalPtr = nullptr; // static, in .cpp?
#elif defined Q_OS_ANDROID
  ArSession* getARscene() const;
  // ArSession* internalPtr = nullptr; // static, in .cpp?
#endif
  // 4. wrapper
  // ArWrapper& getARsceneWrapper() const;

  // properties
  Camera originCamera() const;
  void setOriginCamera(const Camera& originCamera);

  SceneQuickView* sceneView() const;
  // void setSceneView(SceneQuickView* sceneView);
  // read only? In QML: read-write, to set the view in QML. But must be QmlSceneView.

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
  void sceneViewChanged();
  void translationTransformationFactorChanged();
  void renderVideoFeedChanged();
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEW_H
