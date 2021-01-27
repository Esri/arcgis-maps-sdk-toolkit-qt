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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_NORTHARROWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_NORTHARROWCONTROLLER_H

// Qt headers
#include <QObject>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class NorthArrowController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double heading READ heading NOTIFY headingChanged)
public:

  Q_INVOKABLE NorthArrowController(QObject* parent = nullptr);

  ~NorthArrowController();

  QObject* geoView() const;

  void setGeoView(QObject* geoView);

  double heading() const;

signals:
  void geoViewChanged();

  void headingChanged();

public slots:
  void setHeading(double heading);

private:
  QObject* m_geoView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_NORTHARROWCONTROLLER_H
