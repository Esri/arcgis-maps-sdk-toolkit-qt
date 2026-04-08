/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H

// C++ API headers
#include "MapQuickView.h"

// Qt headers
#include <QObject>
#include <QQmlEngine>

// Other headers
#include "BaseDemo.h"

class NorthArrowDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(bool drawingComplete MEMBER m_drawingComplete NOTIFY drawingCompleteChanged)

public:
  Q_INVOKABLE NorthArrowDemo(QObject* parent = nullptr);
  ~NorthArrowDemo() override;

  Q_INVOKABLE double mapViewRotation(Esri::ArcGISRuntime::MapQuickView* mapView) const;
  Q_INVOKABLE void setMapViewRotation(Esri::ArcGISRuntime::MapQuickView* mapView, double degrees);

signals:
  void drawingCompleteChanged();

private:
  void setupMapView_();
  void allowTouchRotationForMapView_();
  void monitorMapViewDrawStatus_();
  void setDrawingComplete_(bool completed);

private:
  QMetaObject::Connection m_drawStatusChangedConnection;
  bool m_drawingComplete = true;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H
