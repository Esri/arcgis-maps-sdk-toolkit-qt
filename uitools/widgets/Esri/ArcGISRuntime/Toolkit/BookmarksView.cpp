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
#include "BookmarksView.h"

#include "ui_BookmarksView.h"

// Toolkit headers
#include "BookmarksViewController.h"
#include "BookmarkListItem.h"

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*!
  \class Esri::ArcGISRuntime::Toolkit::BookmarksView
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \since Esri.ArcGISRuntime 100.15
  \brief The user interface for the BookmarksView.
  The BookmarksView displays a collection of bookmarks in the form of viewpoints from either Webmap/Webscene
  or are programmatically defined. When the user selects a bookmark from the provided list,
  the viewpoint in the geoView is set to the new bookmark's view extent.

  \note By default, the BookmarksView will attempt to fetch the set of developer bookmarks, which require an \l{https://developers.arcgis.com/qt/get-started/#3-access-services-and-content-with-an-api-key}{API key} to access.
  */

  /*!
  \brief Default constructor.
  \list
    \li \a parent Parent widget.
  \endlist

  The view mantains its associated controller, sets up the view itself and its model.
  */
  BookmarksView::BookmarksView(QWidget* parent) :
    QFrame(parent),
    m_controller(new BookmarksViewController(this)),
    m_ui(new Ui::BookmarksView)
  {
    m_ui->setupUi(this);
    m_ui->listView->setModel(m_controller->bookmarks());

    connect(m_ui->listView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& index)
            {
              // Slot that sets the current viewpoint at \a index.
              // When a click event is received (and thus item is selected),
              // the `bookmark` at the given \a index is zoomed to via the controller.
              auto item = m_controller->bookmarks()->element<BookmarkListItem>(index);
              m_controller->zoomToBookmarkExtent(item);
            });
  }

  /*!
    \brief Destructor.
   */
  BookmarksView::~BookmarksView()
  {
    delete m_ui;
  }

  /*!
    \brief Returns the controller.
   */
  BookmarksViewController* BookmarksView::controller() const
  {
    return m_controller;
  }

  /*!
  \brief Set the \c GeoView.
  \list
    \li \a mapView Sets the \c GeoView to a \c MapView.
  \endlist
 */
  void BookmarksView::setMapView(MapGraphicsView* mapView)
  {
    m_controller->setGeoView(mapView);
  }

  /*!
  \brief Set the \c GeoView.
  \list
    \li \a sceneView Sets the \c GeoView to a \c SceneView.
  \endlist
 */
  void BookmarksView::setSceneView(SceneGraphicsView* sceneView)
  {
    m_controller->setGeoView(sceneView);
  }
} //Toolkit
} //ArcGISRuntime
} //Esri
