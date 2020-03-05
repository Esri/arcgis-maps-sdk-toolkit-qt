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

/*!
 * \brief In MVC architecture, this is the controller for the corresponding
 * NorthArrow view.
 * 
 * This controller calculates the current heading from a GeoView, and allows
 * the NorthArrow to apply a given heading to the GeoView.
 */
class NorthArrowController : public QObject
{
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double heading READ heading NOTIFY headingChanged)
  Q_OBJECT
public:
  /*!
   * \brief Constructor
   * \param parent Parent owning QObject.
   */
  Q_INVOKABLE NorthArrowController(QObject* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~NorthArrowController();

  /*!
   * \brief Returns the GeoView as a QObject.
   * \return The current GeoView as a QObject.
   */
  QObject* geoView() const;

  /*!
   * \brief Set the GeoView object this Controller uses.
   * 
   * Internally this is cast to a MapView or SceneView using `qobject_cast`,
   * which is why the paremeter is of form `QObject` and not `GeoView`.
   * 
   * \param geoView Object which must inherit from GeoView* and QObject*. 
   */
  void setGeoView(QObject* geoView);

  /*!
   * \brief The calculated heading of this controller in degrees.
   * \return heading as double
   */
  double heading() const;

signals:
  /*! Emitted when the geoView changes */
  void geoViewChanged();

  /*! Emitted when the heading changes */
  void headingChanged();

public slots:
  /*!
   * \brief Set the heading by rotating the MapView or SceneView camera to the
   * given heading (in degrees).
   */
  void setHeading(double heading);

private:
  QObject* m_geoView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_NORTHARROWCONTROLLER_H
