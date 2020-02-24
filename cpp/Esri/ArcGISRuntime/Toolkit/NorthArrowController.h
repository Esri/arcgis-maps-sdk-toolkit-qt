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
#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

namespace Esri {
namespace ArcGISRuntime {

class GeoView;

namespace Toolkit {

class NorthArrowController : public QObject
{
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double heading READ heading NOTIFY headingChanged)
  Q_OBJECT

public:
  Q_INVOKABLE NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();

  QObject* geoView() const;
  void setGeoView(QObject* mapView);
  Q_SIGNAL void geoViewChanged();

  double heading() const;
  Q_SLOT void setHeading(double heading);
  Q_SIGNAL void headingChanged();

private:
  QObject* m_geoView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NORTHARROWCONTROLLER_H
