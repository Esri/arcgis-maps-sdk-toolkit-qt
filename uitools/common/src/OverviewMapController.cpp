
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
#include "OverviewMapController.h"

#include "SingleShotConnection.h"

// ArcGISRuntime headers
#include <Geometry.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <Map.h>
#include <MapTypes.h>
#include <MapViewTypes.h>
#include <Point.h>
#include <Polygon.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SymbolTypes.h>
#include <Viewpoint.h>

// Qt headers
#include <QtGlobal>
#include <QKeyEvent>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

  OverviewMapController::OverviewMapController(QObject* parent) :
    QObject(parent),
    m_insetView(new MapViewToolkit),
    m_reticle(new Graphic(this))
  {
    // Setup insetViiew defaults.
    m_insetView->setMap(new Map(BasemapStyle::ArcGISTopographic, m_insetView));
    m_insetView->setAttributionTextVisible(false);

    // Disable keyboard interactions, and mouse
    // interactions on devices. Keep track of mouse
    // touches.
    disableInteractions();

    // Add the Graphic to the insetView.
    auto graphicsOverlay = new GraphicsOverlay(m_insetView);
    graphicsOverlay->graphics()->append(m_reticle);
    m_insetView->graphicsOverlays()->append(graphicsOverlay);

    // If the user navigates the inset, apply navigation changes
    // to the main GeoView if applicable.
    connect(m_insetView, &MapViewToolkit::viewpointChanged, this,
            [this]
            {
              if (m_insetView->isNavigating() && !m_setViewpointFuture.isRunning())
              {
                if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
                {
                  applyInsetNavigationToSceneView(sceneView);
                }
                else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
                {
                  applyInsetNavigationToMapView(mapView);
                }
              }
            });
  }

  OverviewMapController::~OverviewMapController()
  {
    m_insetView->deleteLater();
  }

  QObject* OverviewMapController::geoView() const
  {
    return m_geoView;
  }

  void OverviewMapController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);
    }

    m_geoView = geoView;

    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      // If Symbol has not yet been set, we provide a default symbol appropriate for SceneViews.
      if (symbol() == nullptr)
      {
        setSymbol(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, Qt::GlobalColor::red, 16.0f, this));
      }
      // Connect to geoViews's viewpointChanged. Updates insetView when the SceneView-geoView viewpoint changes.
      QObject::connect(sceneView, &SceneViewToolkit::viewpointChanged, this,
                       [this, sceneView]
                       {
                         const Viewpoint viewpoint = sceneView->currentViewpoint(ViewpointType::CenterAndScale);
                         m_reticle->setGeometry(viewpoint.targetGeometry());
                         if (sceneView->isNavigating() && !m_setViewpointInsetFuture.isRunning())
                         {
                           applySceneNavigationToInset(sceneView);
                         }
                       });
      // Create single-shot connection to geoView's drawStatusChanged to ensure OverviewMap updates when the scene initially loads.
      singleShotConnection(sceneView, &SceneViewToolkit::drawStatusChanged, this, [this, sceneView](DrawStatus status)
                           {
                             if (status == DrawStatus::Completed)
                               applySceneNavigationToInset(sceneView);
                           });
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      // If Symbol has not yet been set, we provide a default symbol appropriate for MapViews.
      if (symbol() == nullptr)
      {
        auto symbol = new SimpleFillSymbol(this);
        symbol->setStyle(SimpleFillSymbolStyle::Null);
        symbol->setOutline(new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, Qt::GlobalColor::red, 1.0f, symbol));
        setSymbol(symbol);
      }
      // Connect to geoView's viewpointChanged. Updates insetView when MapView-geoView viewpoint changes.
      QObject::connect(mapView, &MapViewToolkit::viewpointChanged, this,
                       [this, mapView]
                       {
                         m_reticle->setGeometry(mapView->visibleArea());
                         if (mapView->isNavigating() && !m_setViewpointInsetFuture.isRunning())
                         {
                           applyMapNavigationToInset(mapView);
                         }
                       });
      // Create single-shot connection to geoView's drawStatusChanged to ensure OverviewMap updates when the map initially loads.
      singleShotConnection(mapView, &MapViewToolkit::drawStatusChanged, this, [this, mapView](DrawStatus status)
                           {
                             if (status == DrawStatus::Completed)
                               applyMapNavigationToInset(mapView);
                           });
    }
    emit geoViewChanged();
  }

  MapViewToolkit* OverviewMapController::insetView() const
  {
    return m_insetView;
  }

  Symbol* OverviewMapController::symbol() const
  {
    return m_reticle->symbol();
  }

  void OverviewMapController::setSymbol(Symbol* symbol)
  {
    if (m_reticle->symbol() == symbol)
      return;

    if (m_reticle->symbol() && m_reticle->symbol()->parent() == this)
    {
      // If we own the symbol, delete it.
      m_reticle->symbol()->deleteLater();
    }

    m_reticle->setSymbol(symbol);
    emit symbolChanged();
  }

  double OverviewMapController::scaleFactor() const
  {
    return m_scaleFactor;
  }

  void OverviewMapController::setScaleFactor(double scaleFactor)
  {
    if (m_scaleFactor == scaleFactor)
      return;

    m_scaleFactor = scaleFactor;
    emit scaleFactorChanged();

    // Force a redraw of the inset geometry.
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      emit sceneView->viewpointChanged();
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      emit mapView->viewpointChanged();
    }
  }

  void OverviewMapController::applyInsetNavigationToMapView(MapViewToolkit* view)
  {
    // Note we care about rotation in the mapView case.
    const Viewpoint viewpoint = m_insetView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        viewpoint.targetScale() / scaleFactor(),
        viewpoint.rotation()};

    constexpr float animationDuration{0};
    m_setViewpointFuture = view->setViewpointAsync(newViewpoint, animationDuration);
  }

  void OverviewMapController::applyInsetNavigationToSceneView(SceneViewToolkit* view)
  {
    // Note we do not care about rotation in the sceneView case.
    const Viewpoint viewpoint = m_insetView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        viewpoint.targetScale() / scaleFactor()};

    constexpr float animationDuration{0};
    m_setViewpointFuture = view->setViewpointAsync(newViewpoint, animationDuration);
  }

  void OverviewMapController::applyMapNavigationToInset(MapViewToolkit* view)
  {
    // Note we care about rotation in the mapView case.
    const Viewpoint viewpoint = view->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        viewpoint.targetScale() * scaleFactor(),
        viewpoint.rotation()};

    constexpr float animationDuration{0};
    m_setViewpointInsetFuture = m_insetView->setViewpointAsync(newViewpoint, animationDuration);
  }

  void OverviewMapController::applySceneNavigationToInset(SceneViewToolkit* view)
  {
    // Note we do not care about rotation in the sceneView case.
    const Viewpoint viewpoint = view->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        viewpoint.targetScale() * scaleFactor()};

    constexpr float animationDuration{0};
    m_setViewpointInsetFuture = m_insetView->setViewpointAsync(newViewpoint, animationDuration);
  }

  void OverviewMapController::disableInteractions()
  {
    // Disable all keyboard interactions
    QObject::connect(m_insetView, &MapViewToolkit::keyPressed, this, [](QKeyEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::keyReleased, this, [](QKeyEvent& e)
                     {
                       e.accept();
                     });
    // Disable all mouse interactions on devices.
#if defined Q_OS_ANDROID || defined Q_OS_IOS
    QObject::connect(m_insetView, &MapViewToolkit::mouseClicked, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mouseDoubleClicked, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mouseMoved, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mousePressed, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mousePressedAndHeld, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mouseReleased, this, [](QMouseEvent& e)
                     {
                       e.accept();
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mouseWheelChanged, this, [](QWheelEvent& e)
                     {
                       e.accept();
                     });
#endif
  }

} // Esri::ArcGISRuntime::Toolkit
