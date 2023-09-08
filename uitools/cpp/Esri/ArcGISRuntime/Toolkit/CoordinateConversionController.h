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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H

// Toolkit headers
#include "CoordinateConversionOption.h"
#include "Internal/GenericListModel.h"

// Qt headers
#include <QObject>
#include <QString>
#include <QPointF>

// Qt forward declarations
class QAbstractListModel;

// ArcGISRuntime headers
#include <Point.h>

namespace Esri::ArcGISRuntime {

class GeoView;

namespace Toolkit
{

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

  GenericListModel* coordinateFormats() const;

  GenericListModel* conversionResults() const;

  Q_INVOKABLE Esri::ArcGISRuntime::Point currentPoint() const;

  Q_INVOKABLE QPointF screenCoordinate() const;

  double zoomToDistance() const;

  void setZoomToDistance(double distance);

  bool inPickingMode() const;

  void setInPickingMode(bool mode);

signals:
  void geoViewChanged();

  void currentPointChanged(const Esri::ArcGISRuntime::Point& point);

  void zoomToDistanceChanged();

  void inPickingModeChanged();

public slots:

  void setCurrentPoint(const Esri::ArcGISRuntime::Point& point);

  void setCurrentPoint(const QString& point, CoordinateConversionOption* option);

  void setCurrentPoint(const QString& point,
                       const SpatialReference& spatialReference,
                       CoordinateConversionOption* option);

  void zoomToCurrentPoint();

  void addNewCoordinateResultForOption(CoordinateConversionOption* option);

  void removeCoordinateResultAtIndex(int index);

private:
  bool m_screenToLocationInProgress = false;
  Point m_currentPoint;
  double m_zoomToDistance = 0.0;
  GenericListModel* m_coordinateFormats = nullptr;
  GenericListModel* m_conversionResults = nullptr;
  QObject* m_geoView = nullptr;
  bool m_inPickingMode = false;
};

} // Toolkit
} // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H
