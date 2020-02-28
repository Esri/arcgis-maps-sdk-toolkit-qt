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
#include <QPointF>

class QAbstractListModel;

// ArcGISRuntime headers
#include <Point.h>

#include "CoordinateConversionOption.h"

Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Point)

namespace Esri
{
namespace ArcGISRuntime
{

class GeoView;

namespace Toolkit
{

class GenericListModel;

class CoordinateConversionController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double zoomToDistance READ zoomToDistance WRITE setZoomToDistance NOTIFY zoomToDistanceChanged)
  Q_PROPERTY(QAbstractListModel* formats READ coordinateFormats CONSTANT)
  Q_PROPERTY(QAbstractListModel* results READ conversionResults CONSTANT)
  Q_PROPERTY(bool inPickingMode READ inPickingMode WRITE setInPickingMode NOTIFY inPickingModeChanged)
public:
  Q_INVOKABLE CoordinateConversionController(QObject* parent = nullptr);
  ~CoordinateConversionController() override;

  QObject* geoView() const;
  void setGeoView(QObject* mapView);

  QAbstractListModel* coordinateFormats() const;

  QAbstractListModel* conversionResults() const;

  Point currentPoint() const;

  Q_INVOKABLE QPointF screenCoordinate() const;

  double zoomToDistance() const;
  void setZoomToDistance(double distance);

  bool inPickingMode() const;
  void setInPickingMode(bool mode);

signals:
  void geoViewChanged();
  void currentPointChanged(const Point& point);
  void currentPointChanged(QVariant point);
  void zoomToDistanceChanged();
  void inPickingModeChanged();

public slots:
  void setCurrentPoint(const Point& p);
  void setCurrentPoint(const QString& p, CoordinateConversionOption* option);
  void setCurrentPoint(const QString& p, const SpatialReference& spatialReference, CoordinateConversionOption* option);

  void zoomToCurrentPoint();

  void addNewCoordinateResultForOption(CoordinateConversionOption* option);
  void removeCoordinateResultAtIndex(int index);

  void forceUpdateCoordinates();

private:
  Point m_currentPoint;
  double m_zoomToDistance;
  SpatialReference m_conversionSpatialReference;
  GenericListModel* m_coordinateFormats;
  GenericListModel* m_conversionResults;
  QObject* m_geoView;
  bool m_inPickingMode;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONTROLLER_H
