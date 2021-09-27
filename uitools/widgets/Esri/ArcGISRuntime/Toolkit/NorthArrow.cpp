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
#include "NorthArrow.h"

// Toolkit headers
#include "NorthArrowController.h"

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

// Qt headers
#include <QMouseEvent>


namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::NorthArrow
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \brief The \c NorthArrow displays a compass overlaid on the \c GeoView, with 
  the compass heading matching the current rotation of the \c MapView, or 
  \c Camera heading of the \c SceneView. 
  Double-clicking on the \c NorthArrow triggers the heading of the connected
  \c GeoView to be orientated to 0 (North).
  \note default width and height is 48.
 */

/*!
  \brief Constructor
  \list
  \li \a parent Parent widget.
  \endlist
 */
NorthArrow::NorthArrow(QWidget* parent) :
  QLabel(parent),
  m_controller(new NorthArrowController(this))
{
  m_image = QPixmap(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/compass.png");

  if (!m_image.isNull())
  {
    const QSize defaultSize(48,48);
    this->setPixmap(m_image.scaled(defaultSize, Qt::IgnoreAspectRatio));
  }

  connect(m_controller, &NorthArrowController::headingChanged, this, [this]()
  {
    if (m_image.isNull())
      return;

    QMatrix rm;
    rm.rotate(-m_controller->heading());
    const int imageWidth = m_image.width();
    const int imageHeight = m_image.height();
    auto pix = m_image.transformed(rm, Qt::SmoothTransformation);
    pix = pix.copy((pix.width() - imageWidth)/2,
                   (pix.height() - imageHeight)/2,
                   imageWidth,
                   imageHeight);
    this->setPixmap(pix);
  });
}

/*!
  \brief Destructor
 */
NorthArrow::~NorthArrow()
{
}

/*!
  \brief Set the \c GeoView.
  \list
  \li \a mapView Sets the \c GeoView to a \c MapView.
  \endlist
 */
void NorthArrow::setMapView(MapGraphicsView* mapView)
{
  m_controller->setGeoView(mapView);
}

/*!
  \brief Set the \c GeoView.
  \list
    \li \a sceneView Sets the \c GeoView to a \c SceneView.
  \endlist
 */
void NorthArrow::setSceneView(SceneGraphicsView* sceneView)
{
  m_controller->setGeoView(sceneView);
}

/*!
  \internal
  \brief When triggered this will orient the \c GeoView such that this
  \c NorthArrow has a heading of \c 0.
  \list
    \li \a event MouseEvent to accept.
  \endlist
 */
void NorthArrow::mouseDoubleClickEvent(QMouseEvent* event)
{
  // Rotate back to North.
  m_controller->setHeading(0);
  event->accept();
}

/*!
  \brief Returns the controller object driving this widget.
 */
NorthArrowController* NorthArrow::controller() const
{
  return m_controller;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
