
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
#include <FloorFacility.h>
#include <FloorSite.h>
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

// Qt headers
#include <QEvent>
#include <QSortFilterProxyModel>

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {

    /*!
     \internal
     \brief Given a \a modelIndex, for \a model,
     extracts the `T*` pointer from its \c userRole.
     */
    template <typename T>
    T* itemForIndex(QAbstractItemModel* model, const QModelIndex& modelIndex)
    {
      if (!model)
        return nullptr;

      if (modelIndex.isValid())
      {
        return qvariant_cast<T*>(model->data(modelIndex, Qt::UserRole));
      }
      return nullptr;
    }

    /*!
     \internal
     \brief Given some modelId in a given model, returns that items index in the model.
            O(n) search time.
     */
    template <typename T>
    QModelIndex indexForId(QAbstractItemModel* model, const QString& id)
    {
      const int rowCount = model->rowCount();
      for (int i = 0; i < rowCount; ++i)
      {
        const auto index = model->index(i, 0);
        const auto item = itemForIndex<T>(model, index);
        if (item && item->modelId() == id)
          return index;
      }
      return QModelIndex{};
    }

  /*!
    \internal
    \brief Wrap the \a sourceModel with a \l QSortFilterProxyModel that sorts and filters based on the \a filterField textEdited event.
   */
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

    /*!
     \internal
     \brief Helper struct that holds a reference to some value `tracked`, sets `tracked` to `val` on construction,
     and then sets `tracked` back to its initial value on destruction.
     */
    template <typename T>
    struct PushValue
    {
      PushValue(T& tracked, T val) :
        m_tracked(tracked),
        m_state{std::move(m_tracked)}
      {
        m_tracked = std::move(val);
      }

      ~PushValue()
      {
        std::swap(m_tracked, m_state);
      }

      T& m_tracked;
      T m_state;
    };

    /*!
     \internal
     \brief Creates a `PushValue`.
     */
    template <typename T>
    PushValue<T> push_value(T& t, T v)
    {
      return PushValue<T>(t, v);
    }
  } // namespace

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilter
    \brief Displays and filters the available floor aware layers in the current \c GeoModel.

    This component supports manual selection and text-based filtering for sites and facilities.
    Once a site and facility is selected, levels can be selected and the GeoView will be updated.

    In addition to the browsing workflow where sites and facilities are manually selected, automatic facility
    selection based on the GeoView's current viewpoint. This mode is useful when you are exploring the
    GeoView but do not know the name of the site or facility.

    The user interface is driven from the FloorAware data that is available in the GeoModel's FloorManager.

    2D maps and 3D scenes are supported.

    \note Double-clicking a site or facility will automatically open the next pane.
   */

  /*!
    \brief Constructs a new FloorFilter object with a given \a parent.
   */
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
    connect(
        m_ui->allSites, &QCheckBox::clicked, this, [this](bool checked)
        {
          m_controller->setIsSelectedSiteRespected(!checked);
          if (checked)
            m_ui->toolBox->setCurrentIndex(1);
        });

    // Sites setup
    connect(
        m_controller, &FloorFilterController::selectedSiteIdChanged, this, [this](const QString& /*oldId*/, const QString& newId)
        {
          // Set `m_sitesUpdatedFromController` for the duration of scope, then set back to false.
          auto b = push_value(m_sitesUpdatedFromController, true);
          const auto i = indexForId<FloorFilterSiteItem>(m_ui->sitesView->model(), newId);
          m_ui->sitesView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::SelectCurrent);
        });

    connect(
        m_ui->sitesView, &QListView::doubleClicked, this, [this](QModelIndex index)
        {
          if (index.isValid())
            m_ui->toolBox->setCurrentIndex(1);
        });

    connect(
        m_ui->sitesView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& index, const QModelIndex& previous)
        {
          if (index == previous || m_facilitiesUpdatedFromController)
            return;

          if (index == QModelIndex{})
          {
            m_controller->setSelectedSiteId("");
          }
          else
          {
            const auto data = itemForIndex<FloorFilterSiteItem>(m_ui->sitesView->model(), index);
            m_controller->setSelectedSiteId(data->modelId());
            m_controller->zoomToSite(data);
          }
        });

    // Facilities setup.
    connect(
        m_controller, &FloorFilterController::selectedFacilityIdChanged, this, [this](const QString& /*oldId*/, const QString& newId)
        {
          auto b = push_value(m_facilitiesUpdatedFromController, true);
          const auto i = indexForId<FloorFilterFacilityItem>(m_ui->facilitiesView->model(), newId);
          m_ui->facilitiesView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::SelectCurrent);
        });

    connect(
        m_ui->facilitiesView, &QListView::doubleClicked, this, [this](const QModelIndex& index)
        {
          if (index.isValid())
            m_ui->toolBox->setCurrentIndex(2);
        });

    connect(
        m_ui->facilitiesView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& index, const QModelIndex& previous)
        {
          if (index == previous)
            return;

          if (index == QModelIndex{})
          {
            m_controller->setSelectedFacilityId("");
          }
          else
          {
            const auto data = itemForIndex<FloorFilterFacilityItem>(m_ui->facilitiesView->model(), index);
            m_controller->setSelectedFacilityId(data->modelId());
            if (auto site = data->floorFacility()->site())
            {
              m_controller->setSelectedSiteId(site->siteId());
            }

            if (!m_facilitiesUpdatedFromController)
            {
              m_controller->zoomToFacility(data);
            }
          }
        });

    // Levels setup.
    connect(
        m_controller, &FloorFilterController::selectedLevelIdChanged, this, [this](const QString& /*oldId*/, const QString& newId)
        {
          const auto i = indexForId<FloorFilterLevelItem>(m_ui->levelsView->model(), newId);
          m_ui->levelsView->selectionModel()->setCurrentIndex(i, QItemSelectionModel::SelectCurrent);
        });

    connect(
        m_ui->levelsView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& index, const QModelIndex& previous)
        {
          if (index == previous)
            return;

          if (!index.isValid())
          {
            m_controller->setSelectedLevelId("");
          }
          else
          {
            const auto data = itemForIndex<FloorFilterLevelItem>(m_ui->levelsView->model(), index);
            m_controller->setSelectedLevelId(data->modelId());
          }
        });
  }

  /*!
   \brief Destructor.
   */
  FloorFilter::~FloorFilter()
  {
  }

  /*!
    \brief Set the \c GeoView.
    \list
    \li \a mapView Sets the \c GeoView to a \c MapView.
    \endlist
   */
  void FloorFilter::setMapView(MapGraphicsView* mapView)
  {
    m_controller->setGeoView(mapView);
  }

  /*!
    \brief Set the \c GeoView.
    \list
      \li \a sceneView Sets the \c GeoView to a \c SceneView.
    \endlist
   */
  void FloorFilter::setSceneView(SceneGraphicsView* sceneView)
  {
    m_controller->setGeoView(sceneView);
  }

  /*!
    \internal
   */
  FloorFilterController* FloorFilter::controller() const
  {
    return m_controller;
  }

} // Esri::ArcGISRuntime::Toolkit
