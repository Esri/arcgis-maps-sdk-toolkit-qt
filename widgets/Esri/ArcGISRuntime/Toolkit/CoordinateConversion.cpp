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
#include "CoordinateEditDelegate.h"
#include "Flash.h"

// Toolkit Controller headers
#include "Esri/ArcGISRuntime/Toolkit/CoordinateConversionController.h"
#include "Esri/ArcGISRuntime/Toolkit/CoordinateConversionResult.h"
#include "Esri/ArcGISRuntime/Toolkit/GenericListModel.h"
#include "Esri/ArcGISRuntime/Toolkit/GenericTableProxyModel.h"

// Qt headers
#include <QGraphicsEllipseItem>
#include <QItemDelegate>
#include <QMenu>

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

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

CoordinateConversion::~CoordinateConversion()
{
  delete m_ui;
}

void CoordinateConversion::setMapView(MapGraphicsView* mapView)
{
  m_controller->setGeoView(mapView);
}

void CoordinateConversion::setSceneView(SceneGraphicsView* sceneView)
{
  m_controller->setGeoView(sceneView);
}

CoordinateConversionController* CoordinateConversion::controller() const
{
  return m_controller;
}

void CoordinateConversion::addContextMenu(const QPoint& point)
{
  auto menu = new QMenu(m_ui->resultsView);

  // Create context meny for all current Options.
  QMenu* addMenu = new QMenu(menu);
  addMenu->setTitle("Add...");
  auto formats = m_controller->coordinateFormats();
  for (int i = 0; i < formats->rowCount(); ++i)
  {
    auto format = formats->element<CoordinateConversionOption>(formats->index(i));
    addMenu->addAction(format->name(), m_controller,
    [this, menu, f(QPointer<CoordinateConversionOption>(format))]()
    {
      if (f) m_controller->addNewCoordinateResultForOption(f.data());
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
      }
    );
  }

  // Show the menu.
  menu->popup(m_ui->resultsView->mapToGlobal(point));
}

void CoordinateConversion::flash()
{
  delete m_flash;

  auto graphicsView = qobject_cast<QGraphicsView*>(m_controller->geoView());
  if (!graphicsView)
    return;

  const auto point = m_controller->screenCoordinate();
  if (point.isNull() || isnan(point.x()) || isnan(point.y()))
    return;

  m_flash = new Flash();
  m_flash->setRadius(8);
  m_flash->setTargetColor(QApplication::palette().color(QPalette::Highlight));
  m_flash->setPoint(m_controller->screenCoordinate());
  m_flash->play(750);
  graphicsView->scene()->addWidget(m_flash.data());
}

} // Toolkit
} // ArcGISRuntime
} // Esri

