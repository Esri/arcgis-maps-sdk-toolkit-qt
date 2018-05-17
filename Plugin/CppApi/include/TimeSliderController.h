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

#ifndef TIMESLIDERCONTROLLER_H
#define TIMESLIDERCONTROLLER_H

#include "AbstractTool.h"

#include "TimeExtent.h"
#include "TimeValue.h"

namespace Esri
{
namespace ArcGISRuntime
{

class GeoView;
class LayerListModel;
class MapQuickView;
class SceneQuickView;

namespace Toolkit
{

class TimeSliderController : public AbstractTool
{
  Q_OBJECT

  Q_PROPERTY(int numberOfSteps READ numberOfSteps NOTIFY numberOfStepsChanged)
  Q_PROPERTY(QDateTime fullExtentStart READ fullExtentStart NOTIFY fullTimeExtentChanged)
  Q_PROPERTY(QDateTime fullExtentEnd READ fullExtentEnd NOTIFY fullTimeExtentChanged)
  Q_PROPERTY(QDateTime currentExtentStart READ currentExtentStart NOTIFY currentTimeExtentChanged)
  Q_PROPERTY(QDateTime currentExtentEnd READ currentExtentEnd NOTIFY currentTimeExtentChanged)
  Q_PROPERTY(int startStep READ startStep NOTIFY startStepChanged)
  Q_PROPERTY(int endStep READ endStep NOTIFY endStepChanged)

signals:
  void numberOfStepsChanged();
  void fullTimeExtentChanged();
  void currentTimeExtentChanged();
  void startStepChanged();
  void endStepChanged();

public:
  TimeSliderController(QObject* parent = nullptr);
  ~TimeSliderController();

  QString toolName() const override;

  void setProperties(const QVariantMap& properties) override;

  Q_INVOKABLE void setGeoView(QObject* geoView);

  int numberOfSteps() const;

  Esri::ArcGISRuntime::TimeExtent fullTimeExtent() const;
  QDateTime fullExtentStart() const;
  QDateTime fullExtentEnd() const;

  Esri::ArcGISRuntime::TimeExtent currentTimeExtent() const;
  QDateTime currentExtentStart() const;
  QDateTime currentExtentEnd() const;

  int startStep() const;
  int endStep() const;

  Q_INVOKABLE void setStartInterval(int intervalIndex);
  Q_INVOKABLE void setEndInterval(int intervalIndex);

private slots:
  void onOperationalLayersChanged();
  void onMapChanged();
  void onSceneChanged();

private:
  bool setGeoViewInternal(GeoView* geoView);
  void initializeTimeProperties();

  void setNumberOfSteps(int numberOfSteps);
  void setFullTimeExtent(const Esri::ArcGISRuntime::TimeExtent& fullTimeExtent);
  void setStartStep(int startStep);
  void setEndStep(int endStep);
  void calculateStepPositions();

  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
  Esri::ArcGISRuntime::LayerListModel* m_operationalLayers = nullptr;
  Esri::ArcGISRuntime::TimeExtent m_fullTimeExtent;

  int m_numberOfSteps = -1;
  double m_intervalMS = -1;
  int m_startStep = -1;
  int m_endStep = -1;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TIMESLIDERCONTROLLER_H
