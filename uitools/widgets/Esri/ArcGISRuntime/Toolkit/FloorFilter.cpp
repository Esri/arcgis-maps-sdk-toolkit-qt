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
#include <FloorFacility.h>
#include <FloorSite.h>

// Qt headers
#include <QSortFilterProxyModel>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {

    /*!
     \internal
     \brief Given some modelId in a given model, returns that items index in the model.
            O(n) search time.
     */
    template <typename T>
    QModelIndex indexForId(GenericListModel* model, QString id)
    {
      const int rowCount = model->rowCount();
      for (int i = 0; i < rowCount; ++i)
      {
        const auto index = model->index(i);
        const auto item = model->element<T>(index);
        if (item->modelId() == id)
          return index;
      }
      return QModelIndex{};
    }

    QSortFilterProxyModel* wrapFilterModel(QAbstractItemModel* sourceModel, QLineEdit* filterField, FloorFilter* parent)
    {
      auto model = new QSortFilterProxyModel(parent);
      model->setSourceModel(sourceModel);
      model->setSortCaseSensitivity(Qt::CaseInsensitive);
      model->setFilterCaseSensitivity(Qt::CaseInsensitive);

      QObject::connect(filterField, &QLineEdit::textEdited, parent, [model](const QString& text)
                       {
                         model->setFilterFixedString(text);
                       });
      return model;
    }
  }

  FloorFilter::FloorFilter(QWidget* parent) :
    QFrame(parent),
    m_controller(new FloorFilterController(this)),
    m_ui(new Ui::FloorFilter)
  {
    m_ui->setupUi(this);

    m_ui->allSites->setChecked(!m_controller->isSelectedSiteRespected());
    m_ui->sitesView->setModel(wrapFilterModel(m_controller->sites(), m_ui->sitesFiltler, this));
    m_ui->facilitiesView->setModel(wrapFilterModel(m_controller->facilities(), m_ui->facilitiesFilter, this));
    m_ui->levelsView->setModel(m_controller->levels());

    // Changes the contents of the facilities list.
    connect(m_ui->allSites, &QCheckBox::clicked, this, [this](bool checked)
            {
              m_controller->setIsSelectedSiteRespected(!checked);
              if (checked)
                m_ui->toolBox->setCurrentIndex(1);
            });

    // Sites setup
    connect(m_controller, &FloorFilterController::selectedSiteIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterSiteItem>(m_controller->sites(), newId);
              m_ui->sitesView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect);
            });
    connect(m_ui->sitesView, &QListView::doubleClicked, this, [this](QModelIndex index)
            {
              if (index.isValid())
                m_ui->toolBox->setCurrentIndex(1);
            });
    connect(m_ui->sitesView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](QModelIndex index)
            {
              if (index == QModelIndex{})
              {
                m_controller->setSelectedSiteId("");
              }
              else
              {
                const auto model = m_controller->sites();
                const auto data = model->element<FloorFilterSiteItem>(index);
                m_controller->setSelectedSiteId(data->modelId());
                m_controller->zoomToSite(data);
              }
            });

    // Facilities setup.
    connect(m_controller, &FloorFilterController::selectedFacilityIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterFacilityItem>(m_controller->facilities(), newId);
              m_ui->facilitiesView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect);
            });
    connect(m_ui->facilitiesView, &QListView::doubleClicked, this, [this](QModelIndex index)
            {
              if (index.isValid())
                m_ui->toolBox->setCurrentIndex(2);
            });
    connect(m_ui->facilitiesView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](QModelIndex index)
            {
              if (index == QModelIndex{})
              {
                m_controller->setSelectedFacilityId("");
              }
              else
              {
                const auto model = m_controller->facilities();
                const auto data = model->element<FloorFilterFacilityItem>(index);
                m_controller->setSelectedFacilityId(data->modelId());
                if (auto site = data->floorFacility()->site())
                {
                  m_controller->setSelectedSiteId(site->siteId());
                }
                m_controller->zoomToFacility(data);
              }
            });

    // Levels setup.
    connect(m_controller, &FloorFilterController::selectedLevelIdChanged, this, [this](QString /*oldId*/, QString newId)
            {
              const auto i = indexForId<FloorFilterLevelItem>(m_controller->levels(), newId);
              m_ui->levelsView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect);
            });
    connect(m_ui->levelsView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](QModelIndex index)
            {
              if (index == QModelIndex{})
              {
                m_controller->setSelectedLevelId("");
              }
              else
              {
                const auto model = m_controller->levels();
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
