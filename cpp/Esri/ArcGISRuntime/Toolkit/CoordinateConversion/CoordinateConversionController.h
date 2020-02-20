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

#ifndef COORDINATECONVERSIONCONTROLLER_H
#define COORDINATECONVERSIONCONTROLLER_H

// Qt headers
#include <QObject>
#include <QString>

class QAbstractListModel;

// ArcGISRuntime headers
#include <Point.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionOption;

class CoordinateConversionController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(QAbstractListModel* formats READ coordinateFormats CONSTANT)
  Q_PROPERTY(QAbstractListModel* results READ conversionResults CONSTANT)
public:
  Q_INVOKABLE CoordinateConversionController(QObject* parent = nullptr);
  ~CoordinateConversionController() override;

  QObject* geoView() const;
  void setGeoView(QObject* mapView);
  Q_SIGNAL void geoViewChanged();

  QAbstractListModel* coordinateFormats() const;

  QAbstractListModel* conversionResults() const;

  Q_INVOKABLE void zoomTo(Point p);
  //Q_INVOKABLE QString prettyPrint(Point p, Type type);

private:
  QAbstractListModel* m_coordinateFormats;
  QAbstractListModel* m_conversionResults;
  QObject* m_geoView;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONTROLLER_H
