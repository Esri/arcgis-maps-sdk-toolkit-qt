/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
#include "FloorFilter.h"

#include "ui_FloorFilter.h"

// Toolkit headers
#include "FloorFilterController.h"
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    template <typename T>
    int indexForId(GenericListModel* model, QString id)
    {
      const int rowCount = model->rowCount();
      for (int i = 0; i < rowCount; ++i)
      {
        const auto index = model->index(i);
        const auto item = model->element<T>(index);
        if (item->modelId() == id)
          return i;
      }
      return -1;
    }
  }

  FloorFilter::FloorFilter(QWidget* parent) :
    QWidget(parent),
    m_controller(new FloorFilterController(this)),
    m_ui(new Ui::FloorFilter)
  {
    m_ui->setupUi(this);
    m_ui->sitesCombo->setModel(m_controller->sites());
    m_ui->facilitiesCombo->setModel(m_controller->facilities());
    m_ui->levelsCombo->setModel(m_controller->levels());

    // Sites setup
    connect(m_controller, &FloorFilterController::selectedSiteIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterSiteItem>(m_controller->sites(), newId);
              m_ui->sitesCombo->setCurrentIndex(i);
            });
    connect(m_ui->sitesCombo, qOverload<int>(&QComboBox::activated), this, [this](int i)
            {
              if (i == -1)
              {
                m_controller->setSelectedSiteId("");
              }
              else
              {
                const auto model = m_controller->sites();
                const auto index = model->index(i);
                const auto data = model->element<FloorFilterSiteItem>(index);
                m_controller->setSelectedSiteId(data->modelId());
                m_controller->zoomToSite(data);
              }
            });

    // Facilities setup.
    connect(m_controller, &FloorFilterController::selectedFacilityIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterFacilityItem>(m_controller->facilities(), newId);
              m_ui->facilitiesCombo->setCurrentIndex(i);
            });
    connect(m_ui->facilitiesCombo, qOverload<int>(&QComboBox::activated), this, [this](int i)
            {
              if (i == -1)
              {
                m_controller->setSelectedFacilityId("");
              }
              else
              {
                const auto model = m_controller->facilities();
                const auto index = model->index(i);
                const auto data = model->element<FloorFilterFacilityItem>(index);
                m_controller->setSelectedFacilityId(data->modelId());
                m_controller->zoomToFacility(data);
              }
            });

    // Levels setup.
    connect(m_controller, &FloorFilterController::selectedLevelIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterLevelItem>(m_controller->levels(), newId);
              m_ui->levelsCombo->setCurrentIndex(i);
            });
    connect(m_ui->levelsCombo, qOverload<int>(&QComboBox::activated), this, [this](int i)
            {
              if (i == -1)
              {
                m_controller->setSelectedLevelId("");
              }
              else
              {
                const auto model = m_controller->levels();
                const auto index = model->index(i);
                const auto data = model->element<FloorFilterLevelItem>(index);
                m_controller->setSelectedLevelId(data->modelId());
              }
            });
  }

  FloorFilter::~FloorFilter()
  {
    delete m_ui;
  }

  void FloorFilter::setMapView(MapGraphicsView* mapView)
  {
    m_controller->setGeoView(mapView);
  }

  void FloorFilter::setSceneView(SceneGraphicsView* sceneView)
  {
    m_controller->setGeoView(sceneView);
  }

  FloorFilterController* FloorFilter::controller() const
  {
    return m_controller;
  }

} //Toolkit
} //ArcGISRuntime
} //Esri
