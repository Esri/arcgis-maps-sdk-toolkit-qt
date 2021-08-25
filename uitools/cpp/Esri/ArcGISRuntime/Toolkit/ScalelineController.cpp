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
#include "ScalelineController.h"

// C++ headers
#include <cmath>

// ArcGISRuntime headers
#include <GeometryEngine.h>
#include <PolylineBuilder.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {

    constexpr double MULTIPLIERS[] = {
        1,
        1.2,
        1.25,
        1.5,
        1.75,
        2.0,
        2.4,
        2.5,
        3,
        3.75,
        4,
        5,
        6,
        7.5,
        8.0,
        9.0,
        10,
    };

    double selectMultiplierData(double distance, double magnitude)
    {
      double residual = distance / magnitude;
      return std::accumulate(
          std::cbegin(MULTIPLIERS),
          std::cend(MULTIPLIERS), MULTIPLIERS[0],
          [residual](double i, double m)
          {
            return m < residual && m > i ? m : i;
          });
    }

    /*
      Calculates the "magnitude" used when calculating the length of a scalebar or the number of segments. This is the
      largest power of 10 that's less than or equal to a given distance
    */
    double calculateMagnitude(double distance)
    {
      return pow(10, floor(log10(distance)));
    }

    LinearUnit selectLinearUnit(double distance, UnitSystem unitSystem)
    {
      switch (unitSystem)
      {
      case UnitSystem::Imperial:
        // use MILES if at least half a mile
        if (distance >= 2640)
        {
          return LinearUnit{LinearUnitId::Miles};
        }
        return LinearUnit{LinearUnitId::Feet};

      case UnitSystem::Metric:
      default:
        // use KILOMETERS if at least one kilometer
        if (distance >= 1000)
        {
          return LinearUnit{LinearUnitId::Kilometers};
        }
        return LinearUnit{LinearUnitId::Meters};
      }
    }
  }

  /*!
  \class Esri::ArcGISRuntime::Toolkit::ScalelineController
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief In MVC architecture, this is the controller for the corresponding
  \c Scaleline view.
 */

  /*!
  \brief Constructor
  \list
    \li \a parent Parent owning \c QObject.
  \endlist
 */
  ScalelineController::ScalelineController(QObject* parent) :
    QObject(parent)
  {
  }

  /*!
  \brief Destructor.
 */
  ScalelineController::~ScalelineController()
  {
  }

  QObject* ScalelineController::mapView() const
  {
    return m_mapView;
  }

  void ScalelineController::setMapView(QObject* qObject)
  {
    auto mapView = qobject_cast<MapViewToolkit*>(qObject);

    if (m_mapView == mapView)
      return;

    m_mapView = mapView;
    emit mapViewChanged();
  }

  int ScalelineController::unitSystem()
  {
    return static_cast<int>(m_unitSystem);
  }

  void ScalelineController::setUnitSystem(int unitSystem)
  {
    if (static_cast<int>(m_unitSystem) == unitSystem)
      return;

    m_unitSystem = static_cast<UnitSystem>(unitSystem);

    switch (m_unitSystem)
    {
    case UnitSystem::Imperial:
      m_baseUnit = LinearUnit{LinearUnitId::Feet};
      break;
    case UnitSystem::Metric:
    default:
      m_baseUnit = LinearUnit{LinearUnitId::Meters};
      break;
    }

    emit unitSystemChanged();
  }

  double ScalelineController::calculateBestScalebarLength(double maxLength)
  {
    return calculateBestScalebarLength(maxLength, m_baseUnit);
  }

  double ScalelineController::calculateBestScalebarLength(double maxLength, LinearUnit unit)
  {
    double magnitude = calculateMagnitude(maxLength);
    double multiplier = selectMultiplierData(maxLength, magnitude);

    double bestLength = multiplier * magnitude;

    // If using imperial units, check if the number of feet is greater than the threshold for using feet
    if (unit.linearUnitId() == LinearUnitId::Feet)
    {
      LinearUnit displayUnits = selectLinearUnit(bestLength, UnitSystem::Imperial);
      if (unit.linearUnitId() != displayUnits.linearUnitId())
      {
        // Recalculate the best length in miles
        bestLength = calculateBestScalebarLength(unit.convertTo(displayUnits, maxLength), displayUnits);
        // But convert that back to feet because the caller is using feet
        return displayUnits.convertTo(unit, bestLength);
      }
    }
    return bestLength;
  }

  double ScalelineController::calculateDistance(double width)
  {
    double distance = 0.0;

    if (!m_mapView)
    {
      return distance;
    }

    Polygon visibleArea = m_mapView->visibleArea();
    if (!visibleArea.isEmpty())
    {
      Point mapCenter = visibleArea.extent().center();
      if (!mapCenter.isEmpty())
      {
        double maxPlanarWidth = m_mapView->unitsPerDIP() * width;
        Point point1 = Point(mapCenter.x() - (maxPlanarWidth / 2.0), mapCenter.y());
        Point point2 = Point(mapCenter.x() + (maxPlanarWidth / 2.0), mapCenter.y());

        auto builder = std::make_unique<PolylineBuilder>(m_mapView->spatialReference());
        builder->addPoint(point1);
        builder->addPoint(mapCenter);
        builder->addPoint(point2);

        distance = GeometryEngine::lengthGeodetic(builder->toGeometry(), m_baseUnit, GeodeticCurveType::Geodesic);
      }
    }

    return distance;
  }

  double ScalelineController::calculateDisplayWidth(double displayDistance, double maximumDistance, double availableWidth)
  {
    return displayDistance / maximumDistance * availableWidth;
  }

  QString ScalelineController::calculateDistanceInDisplayUnits(double distance, int unitSystem)
  {
    return calculateDistanceInDisplayUnits(distance, static_cast<UnitSystem>(unitSystem));
  }

  /**
   * Calculates the distance value to display in the correct units.
   *
   * @param distance the distance in the base unit
   * @param baseUnit the base unit
   * @param displayUnit the display unit
   * @return the distance to display
   * @throws NullPointerException if baseUnit is null
   * @throws NullPointerException if displayUnit is null
   * @since 100.2.1
   */
  QString ScalelineController::calculateDistanceInDisplayUnits(double distance, UnitSystem unitSystem)
  {
    double displayDistance = distance;
    LinearUnit displayUnit = selectLinearUnit(distance, unitSystem);
    if (displayUnit.unitId() != m_baseUnit.unitId())
    {
      displayDistance = m_baseUnit.convertTo(displayUnit, displayDistance);
    }

    return QString{"%1 %2"}.arg(displayDistance).arg(displayUnit.abbreviation());
  }

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::ScalelineController::geoViewChanged()
  \brief Emitted when the geoView changes.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::ScalelineController::geoView
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
