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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTERCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTERCONTROLLER_H

// Toolkit headers
#include "Internal/GenericListModel.h"

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class FloorFacility;
  class FloorSite;
  class FloorLevel;

  namespace Toolkit {

    class FloorFilterController : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
      Q_PROPERTY(QString selectedFacilityId READ selectedFacilityId WRITE setSelectedFacilityId NOTIFY selectedFacilityIdChanged)
      Q_PROPERTY(QString selectedLevelId READ selectedLevelId WRITE setSelectedLevelId NOTIFY selectedLevelIdChanged)
      Q_PROPERTY(QString selectedSiteId READ selectedSiteId WRITE setSelectedSiteId NOTIFY selectedSiteIdChanged)
      Q_PROPERTY(QAbstractListModel* levels READ levels CONSTANT)
      Q_PROPERTY(QAbstractListModel* facilities READ facilities CONSTANT)
      Q_PROPERTY(QAbstractListModel* sites READ sites CONSTANT)
    public:
      Q_INVOKABLE explicit FloorFilterController(QObject* parent = nullptr);
      ~FloorFilterController() override;

      QObject* geoView() const;
      void setGeoView(QObject* geoView);

      QString selectedFacilityId() const;
      void setSelectedFacilityId(QString selectedFacilityId);

      QString selectedLevelId() const;
      void setSelectedLevelId(QString selectedLevelId);

      QString selectedSiteId() const;
      void setSelectedSiteId(QString selectedSiteId);

      GenericListModel* levels() const;
      GenericListModel* sites() const;
      GenericListModel* facilities() const;

    signals:
      void geoViewChanged();
      void selectedFacilityIdChanged();
      void selectedLevelIdChanged();
      void selectedSiteIdChanged();
      void selectedChanged();

    private slots:
      void populateLevelsForSelectedFacility();
      void populateFacilitiesForSelectedSite();
      void populateSites();

    private:
      FloorFacility* selectedFacility() const;
      FloorSite* selectedSite() const;
      FloorLevel* selectedLevel() const;

    private:
      QObject* m_geoView{nullptr};
      GenericListModel* m_levels{nullptr};
      GenericListModel* m_facilities{nullptr};
      GenericListModel* m_sites{nullptr};
      QString m_selectedFacilityId;
      QString m_selectedLevelId;
      QString m_selectedSiteId;
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTERCONTROLLER_H
