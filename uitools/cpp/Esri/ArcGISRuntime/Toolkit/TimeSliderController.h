/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_TIMESLIDERCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_TIMESLIDERCONTROLLER_H

// ArcGISRuntime headers
#include <TimeExtent.h>
#include <TimeValue.h>

// Qt headers
#include <QDateTime>
#include <QObject>
#include <QPointer>

namespace Esri::ArcGISRuntime {

class GeoView;
class LayerListModel;
class MapView;
class SceneView;

namespace Toolkit
{

class TimeSliderController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(int numberOfSteps READ numberOfSteps NOTIFY extentsChanged)
  Q_PROPERTY(int startStep READ startStep NOTIFY stepsChanged)
  Q_PROPERTY(int endStep READ endStep NOTIFY stepsChanged)
public:
  explicit Q_INVOKABLE TimeSliderController(QObject* parent = nullptr);

  ~TimeSliderController() override;

  QObject* geoView() const;
  void setGeoView(QObject* geoView);

  TimeExtent fullTimeExtent() const;

  TimeValue timeInterval() const;

  int numberOfSteps() const;

  int startStep() const;
  void setStartStep(int index);

  int endStep() const;
  void setEndStep(int index);

  Q_INVOKABLE QDateTime timeForStep(int step) const;

  void setSteps(std::pair<int, int> steps);
  Q_INVOKABLE void setSteps(int startStep, int endStep);

signals:
  void geoViewChanged();
  void extentsChanged();
  void stepsChanged();

private slots:
    void initializeTimeProperties();

private:
  void initializeTimeProperties(LayerListModel* operationalLayers);
  void disconnectAllLayers();
  std::pair<int, int> stepsForGeoViewExtent() const;

private:
  std::pair<int, int> m_steps {0, 0};
  QPointer<QObject> m_geoView = nullptr;
  QPointer<LayerListModel> m_operationalLayers;
};

} // Toolkit
} // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_TIMESLIDERCONTROLLER_H
