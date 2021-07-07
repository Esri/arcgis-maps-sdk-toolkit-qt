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

// Toolkit headers
#include "Internal/GeoViews.h"

// ArcGISRuntime headers
#include <Graphic.h>
#include <GraphicsOverlay.h>
#include <Map.h>
#include <MapTypes.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <Viewpoint.h>

// Qt headers
#include <QtGlobal>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {

    /*!
      \internal
      Connect to MapView's viewpointChanged. Updates insetView when MapView viewpoint changes.
      Note, we display the visible area as the reticle, and we apply viewpoint rotations.
     */
    void connectToMapView(OverviewMapController* self, MapViewToolkit* view, Graphic* reticle)
    {
      QObject::connect(view, &MapViewToolkit::viewpointChanged, self,
                       [self, view, reticle]
                       {
                         reticle->setGeometry(view->visibleArea());
                         if (view->isNavigating())
                         {
                           const Viewpoint viewpoint = view->currentViewpoint(ViewpointType::CenterAndScale);
                           const Viewpoint newViewpoint{
                               geometry_cast<Point>(viewpoint.targetGeometry()),
                               viewpoint.targetScale() * self->scaleFactor(),
                               viewpoint.rotation()};

                           constexpr float animationDuration {0};
                           self->insetView()->setViewpoint(newViewpoint, animationDuration);
                         }
                       });
    }

    /*!
      \internal
      Connect to SceneView's viewpointChanged. Updates insetView when MapView viewpoint changes.
      Note, we display center and scale as the reticle, and we ignore viewpoint rotations.
     */
    void connectToSceneView(OverviewMapController* self, SceneViewToolkit* view, Graphic* reticle)
    {
      QObject::connect(view, &SceneViewToolkit::viewpointChanged, self,
                       [self, view, reticle]
                       {
                         const Viewpoint viewpoint = view->currentViewpoint(ViewpointType::CenterAndScale);
                         reticle->setGeometry(viewpoint.targetGeometry());
                         if (view->isNavigating())
                         {
                           const Viewpoint newViewpoint{
                               geometry_cast<Point>(viewpoint.targetGeometry()),
                               viewpoint.targetScale() * self->scaleFactor()};

                           constexpr float animationDuration {0};
                           self->insetView()->setViewpoint(newViewpoint, animationDuration);
                         }
                       });
    }
  }

  /*!
    \internal
    \brief If the inset view was navigated by the user, then this is called
    to update the geoView viewpoint if it is a MapView.
   */
  void applyInsetNavigationToMapView(OverviewMapController* self, MapViewToolkit* view)
  {
    auto insetView = self->insetView();
    const Viewpoint viewpoint = insetView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        view->currentViewpoint(ViewpointType::CenterAndScale).targetScale(),
        viewpoint.rotation()};

    constexpr float animationDuration {0};
    view->setViewpoint(newViewpoint, animationDuration);
  }

  /*!
    \internal
    \brief If the inset view was navigated by the user, then this is called
    to update the geoView viewpoint if it is a SceneView.
   */
  void applyInsetNavigationToSceneView(OverviewMapController* self, SceneViewToolkit* view)
  {
    auto insetView = self->insetView();
    const Viewpoint viewpoint = insetView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewpoint{
        geometry_cast<Point>(viewpoint.targetGeometry()),
        view->currentViewpoint(ViewpointType::CenterAndScale).targetScale()};

    constexpr float animationDuration {0};
    view->setViewpoint(newViewpoint, animationDuration);
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::OverviewMapController
    \brief The controller part of a OverviewMap. This class handles the
    management of the inset MapView, including symbol styling and navigation.
   */

  /*!
    \brief Constructs a new OverviewMapController object with a given \a parent.
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
    handleInteractions();

    // Add the Graphic to the insetView.
    auto graphicsOverlay = new GraphicsOverlay(m_insetView);
    graphicsOverlay->graphics()->append(m_reticle);
    m_insetView->graphicsOverlays()->append(graphicsOverlay);

    // If the user navigates the inset, apply navigation changes 
    // to the main GeoView if applicable.
    connect(m_insetView, &MapViewToolkit::viewpointChanged, this,
            [this]
            {
              if (m_insetView->isNavigating() && m_insetHasMouse)
              {
                if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
                {
                  applyInsetNavigationToSceneView(this, sceneView);
                }
                else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
                {
                  applyInsetNavigationToMapView(this, mapView);
                }
              }
            });
  }

  /*!
    \brief Destructor.
   */
  OverviewMapController::~OverviewMapController()
  {
    m_insetView->deleteLater();
  }

  /*!
    \brief Returns the \c GeoView as a \c QObject.
   */
  QObject* OverviewMapController::geoView() const
  {
    return m_geoView;
  }

  /*!
  \brief Set the GeoView object this Controller uses.
  
  Internally this is cast to a \c MapView or \c SceneView using \c qobject_cast,
  which is why the paremeter is of form \c QObject and not \c GeoView.
  
  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and 
        \c{QObject*}. 
  \endlist
  */
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
      connectToSceneView(this, sceneView, m_reticle);
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
      connectToMapView(this, mapView, m_reticle);
    }
    emit geoViewChanged();
  }

  /*!
    \brief Returns a MapView owned by this controller which represents the 
    underlying inset of the OverviewMap tool.

    For the C++/Quick toolkit this is a MapQuickView, for the C++/Widgets toolkit 
    this is a MapGraphicsView.
   */
  MapViewToolkit* OverviewMapController::insetView() const
  {
    return m_insetView;
  }

  /*!
    \brief Returns the symbol used to represent the GeoView's current viewpoint.

    If \l OverviewMapController::geoView is not set, then the symbol will 
    return \c{nullptr} by default. 
    
    If \l OverviewMapController::geoView is set, and a custom symbol was not applied, 
    then a default will be provided.

    For MapView, the default is an empty SimpleFillSymbol with a 1 point red outline;
    for SceneView, it will be a red, cross-hair SimpleMarkerSymbol.
   */
  Symbol* OverviewMapController::symbol() const
  {
    return m_reticle->symbol();
  }

  /*!
    \brief Sets the symbol used to represent the GeoView's current viewpoint.
   */
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

  /*!
    \brief Returns the amount to scale the OverviewMap compared to the geoView.
    Defaults to 25.0.
   */
  double OverviewMapController::scaleFactor() const
  {
    return m_scaleFactor;
  }

  /*!
    \brief Sets the amount to scale the OverviewMap compared to the geoView.
   */
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

  /*!
    \internal
    Disables certain interactions on the insetView.
   */
  void OverviewMapController::handleInteractions()
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
    QObject::connect(m_insetView, &MapViewToolkit::mouseWheelChanged, this, [](QWheelEvent& e)
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
#else
    QObject::connect(m_insetView, &MapViewToolkit::mousePressed, this, [this](QMouseEvent& /*e*/)
                     {
                       m_insetHasMouse = true;
                     });
    QObject::connect(m_insetView, &MapViewToolkit::mouseReleased, this, [this](QMouseEvent& /*e*/)
                     {
                       m_insetHasMouse = false;
                     });
#endif
  }

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::OverviewMapController::geoViewChanged()
  \brief Emitted when the geoView has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::OverviewMapController::scaleFactorChanged()
  \brief Emitted when the scaleFactor has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::OverviewMapController::symbolChanged()
  \brief Emitted when the symbol has changed.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::OverviewMapController::geoView
  \brief The geoview the controller is listening to for view changes.
  \sa Esri::ArcGISRuntime::Toolkit::OverviewMapController::geoView()
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::OverviewMapController::insetView
  \brief MapView which represents an overview/inset of the current viewpoint of the geoView.
  \sa Esri::ArcGISRuntime::Toolkit::OverviewMapController::insetView()
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::OverviewMapController::scaleFactor
  \brief The scaleFactor between the geoView's viewpoint and the insetView's viewpoint.
  \sa Esri::ArcGISRuntime::Toolkit::OverviewMapController::scaleFactor()
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
