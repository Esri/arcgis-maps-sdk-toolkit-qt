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
#include "CoordinateConversion.h"

#include "ui_CoordinateConversion.h"

// Toolkit headers
#include "Internal/CoordinateEditDelegate.h"
#include "Internal/Flash.h"

// Toolkit Controller headers
#include "CoordinateConversionController.h"
#include "CoordinateConversionResult.h"
#include "Internal/GenericListModel.h"
#include "Internal/GenericTableProxyModel.h"

// ArcGISRuntime headers
#include "MapGraphicsView.h"
#include "SceneGraphicsView.h"

// Qt headers
#include <QGraphicsEllipseItem>
#include <QItemDelegate>
#include <QMenu>

// std headers
#include <cmath>

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversion
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \brief The user interface for the coordinate conversion tool.
  This tool allows a user to select a point on the map or to enter a point by
  text entry.
  The point may be highlighted by zooming into that point, or by highlighting
  the point on the \c GeoView.
  A list of different coordinate formats represnenting the same point are
  available to the user.
 */

  /*!
    \brief Constructor.
    \list
      \li \a parent Parent widget.
    \endlist
   */
  CoordinateConversion::CoordinateConversion(QWidget* parent) :
    QFrame(parent),
    m_controller(new CoordinateConversionController(this)),
    m_flash(nullptr),
    m_ui(new Ui::CoordinateConversion)
  {
    m_ui->setupUi(this);

    auto coordinateEditDelegate = new CoordinateEditDelegate(m_ui->resultsView);
    coordinateEditDelegate->setController(m_controller);
    m_ui->resultsView->setItemDelegate(coordinateEditDelegate);

    auto tableModel = new GenericTableProxyModel(this);
    tableModel->setSourceModel(m_controller->conversionResults());
    m_ui->resultsView->setModel(tableModel);
    m_ui->resultsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_ui->resultsView->hideColumn(2);

    m_ui->resultsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->resultsView, &QTableView::customContextMenuRequested,
            this, &CoordinateConversion::addContextMenu);

    connect(m_ui->captureMode, &QCheckBox::toggled, m_controller,
            &CoordinateConversionController::setInPickingMode);

    connect(m_ui->zoomTo, &QPushButton::clicked, m_controller,
            &CoordinateConversionController::zoomToCurrentPoint);

    connect(m_ui->flash, &QPushButton::clicked, this,
            &CoordinateConversion::flash);
  }

  /*!
  \brief Destructor.
 */
  CoordinateConversion::~CoordinateConversion()
  {
    delete m_ui;
  }

  /*!
  \brief Set the \c GeoView.
  \list
    \li \a mapView Sets the GeoView to a \c MapView.
  \endlist
 */
  void CoordinateConversion::setMapView(MapGraphicsView* mapView)
  {
    m_controller->setGeoView(mapView);
  }

  /*!
  \brief Set the \c GeoView.
  \list
  \li \a sceneView Sets the \c GeoView to a \c SceneView.
  \endlist
 */
  void CoordinateConversion::setSceneView(SceneGraphicsView* sceneView)
  {
    m_controller->setGeoView(sceneView);
  }

  /*!
  \brief Returns the controller object driving this widget.
 */
  CoordinateConversionController* CoordinateConversion::controller() const
  {
    return m_controller;
  }

  /*!
  \internal
  \brief Bring up the context menu at the given point.
  \list
  \li \a point Point to show menu
  \endlist
 */
  void CoordinateConversion::addContextMenu(const QPoint& point)
  {
    auto menu = new QMenu(m_ui->resultsView);

    // Create context meny for all current Options.
    QMenu* addMenu = new QMenu(menu);
    addMenu->setTitle("Add...");
    auto formats = m_controller->coordinateFormats();
    const int rowCount = formats->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
      auto format = formats->element<CoordinateConversionOption>(formats->index(i));
      addMenu->addAction(format->name(), m_controller,
                         [this, menu, f(QPointer<CoordinateConversionOption>(format))]()
                         {
                           if (f)
                             m_controller->addNewCoordinateResultForOption(f.data());

                           menu->deleteLater();
                         });
    }
    menu->addMenu(addMenu);

    // If the user clicked on an index, give them the option to delete it.
    auto index = m_ui->resultsView->indexAt(point);
    if (index.isValid())
    {
      auto action = menu->addAction("Delete");
      connect(action, &QAction::triggered, menu,
              [this, index, menu]()
              {
                m_ui->resultsView->model()->removeRow(index.row());
                menu->deleteLater();
              });
    }

    // Show the menu.
    menu->popup(m_ui->resultsView->mapToGlobal(point));
  }

  /*!
  \internal
  \brief Flash the dot on the screen at current location.
 */
  void CoordinateConversion::flash()
  {
    delete m_flash;

    auto graphicsView = qobject_cast<QGraphicsView*>(m_controller->geoView());
    if (!graphicsView)
      return;

    const auto point = m_controller->screenCoordinate();
    if (point.isNull() || std::isnan(point.x()) || std::isnan(point.y()))
      return;

    m_flash = new Flash();
    m_flash->setRadius(8);
    m_flash->setTargetColor(QApplication::palette().color(QPalette::Highlight));
    m_flash->setPoint(m_controller->screenCoordinate());
    m_flash->play(750);
    graphicsView->scene()->addWidget(m_flash.data());
  }

} // Esri::ArcGISRuntime::Toolkit
