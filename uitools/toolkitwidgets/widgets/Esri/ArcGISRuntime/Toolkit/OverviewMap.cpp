/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#include "OverviewMap.h"

#include "ui_OverviewMap.h"

// Toolkit headers
#include "OverviewMapController.h"

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

// Qt headers
#include <QGridLayout>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::OverviewMap
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \brief Defines a small "overview" (or "inset") map displaying a representation of the current viewpoint of the given GeoView.
   For MapViews, the current viewpoint will be represented by a polygon displaying the visible area of the MapView. For SceneViews,
   the current viewpoint will be represented by a reticle (crosshairs) displaying the viewpoint center.

  \note By default, the OverviewMap will attempt to use an ArcGIS Topographic basemap, which requires an \l{https://developers.arcgis.com/qt/get-started/#3-access-services-and-content-with-an-api-key}{API key} to access.
 */

/*!
  \brief Constructor
  \list
  \li \a parent Parent widget.
  \endlist
 */
OverviewMap::OverviewMap(QWidget* parent) :
  QWidget(parent),
  m_ui(new Ui::OverviewMap),
  m_controller(new OverviewMapController(this))
{
  m_ui->setupUi(this);
  m_ui->gridLayout->addWidget(m_controller->insetView(),  0, 0, 1, 1);
}

/*!
  \brief Destructor
 */
OverviewMap::~OverviewMap()
{
  delete m_ui;
}

/*!
  \brief Set the \c GeoView.
  \list
  \li \a mapView Sets the \c GeoView to a \c MapView.
  \endlist
 */
void OverviewMap::setGeoView(MapGraphicsView* mapView)
{
  m_controller->setGeoView(mapView);
}

/*!
  \brief Set the \c GeoView.
  \list
    \li \a sceneView Sets the \c GeoView to a \c SceneView.
  \endlist
 */
void OverviewMap::setGeoView(SceneGraphicsView* sceneView)
{
  m_controller->setGeoView(sceneView);
}

/*!
  \brief Returns the controller object driving this widget.
 */
OverviewMapController* OverviewMap::controller() const
{
  return m_controller;
}

} // Esri::ArcGISRuntime::Toolkit
