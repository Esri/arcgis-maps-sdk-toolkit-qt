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
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "Internal/GenericListModel.h"

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {

  class Envelope;

  namespace Toolkit {
    class FloorFilterFacilityItem;
    class FloorFilterSiteItem;
    class FloorFilterLevelItem;

    class FloorFilterController : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
      Q_PROPERTY(bool selectedSiteRespected READ isSelectedSiteRespected WRITE setIsSelectedSiteRespected NOTIFY isSelectedSiteRespectedChanged)
      Q_PROPERTY(FloorFilterFacilityItem* selectedFacility READ selectedFacility NOTIFY selectedFacilityIdChanged)
      Q_PROPERTY(QString selectedFacilityId READ selectedFacilityId WRITE setSelectedFacilityId NOTIFY selectedFacilityIdChanged)
      Q_PROPERTY(FloorFilterLevelItem* selectedLevel READ selectedLevel NOTIFY selectedLevelIdChanged)
      Q_PROPERTY(QString selectedLevelId READ selectedLevelId WRITE setSelectedLevelId NOTIFY selectedLevelIdChanged)
      Q_PROPERTY(FloorFilterSiteItem* selectedSite READ selectedSite NOTIFY selectedSiteIdChanged)
      Q_PROPERTY(QString selectedSiteId READ selectedSiteId WRITE setSelectedSiteId NOTIFY selectedSiteIdChanged)
      Q_PROPERTY(QAbstractListModel* levels READ levels CONSTANT)
      Q_PROPERTY(QAbstractListModel* facilities READ facilities CONSTANT)
      Q_PROPERTY(QAbstractListModel* sites READ sites CONSTANT)
      Q_PROPERTY(UpdateLevelsMode updateLevelsMode READ updateLevelsMode WRITE setUpdateLevelsMode NOTIFY updateLevelsModeChanged)
      Q_PROPERTY(AutomaticSelectionMode automaticSelectionMode READ automaticSelectionMode WRITE setAutomaticSelectionMode NOTIFY automaticSelectionModeChanged)
    public:
      enum class UpdateLevelsMode
      {
        AllLevelsMatchingVerticalOrder
      };
      Q_ENUM(UpdateLevelsMode)

      enum class AutomaticSelectionMode
      {
        Never,
        Always,
        AlwaysNonClearing
      };
      Q_ENUM(AutomaticSelectionMode)

      Q_INVOKABLE explicit FloorFilterController(QObject* parent = nullptr);
      ~FloorFilterController() override;

      bool isSelectedSiteRespected() const;
      void setIsSelectedSiteRespected(bool isSelectedSiteRespected);

      QObject* geoView() const;
      void setGeoView(QObject* geoView);

      QString selectedSiteId() const;
      Q_INVOKABLE void setSelectedSiteId(QString selectedSiteId);

      QString selectedFacilityId() const;
      Q_INVOKABLE void setSelectedFacilityId(QString selectedFacilityId);

      QString selectedLevelId() const;
      Q_INVOKABLE void setSelectedLevelId(QString selectedLevelId);

      UpdateLevelsMode updateLevelsMode() const;
      void setUpdateLevelsMode(UpdateLevelsMode updateLevelsMode);

      AutomaticSelectionMode automaticSelectionMode() const;
      void setAutomaticSelectionMode(AutomaticSelectionMode automaticSelectionMode);

      GenericListModel* levels() const;
      GenericListModel* sites() const;
      GenericListModel* facilities() const;

      Q_INVOKABLE void zoomToFacility(const QString& facilityId);
      void zoomToFacility(FloorFilterFacilityItem* facilityItem);

      Q_INVOKABLE void zoomToSite(const QString& siteId);
      void zoomToSite(FloorFilterSiteItem* siteItem);

      Q_INVOKABLE Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem* facility(const QString& facilityId) const;

      Q_INVOKABLE Esri::ArcGISRuntime::Toolkit::FloorFilterSiteItem* site(const QString& siteId) const;

      Q_INVOKABLE Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem* level(const QString& levelId) const;

    public slots:
      void tryUpdateSelection();

    signals:
      void geoViewChanged();
      void selectedSiteIdChanged(QString oldId, QString newId);
      void selectedFacilityIdChanged(QString oldId, QString newId);
      void selectedLevelIdChanged(QString oldId, QString newId);
      void isSelectedSiteRespectedChanged();
      void updateLevelsModeChanged();
      void automaticSelectionModeChanged();

    private slots:
      void populateLevelsForSelectedFacility();
      void populateFacilitiesForSelectedSite();
      void populateSites();

    private:
      FloorFilterFacilityItem* selectedFacility() const;
      FloorFilterSiteItem* selectedSite() const;
      FloorFilterLevelItem* selectedLevel() const;
      void zoomToEnvelope(const Envelope& envelope);

    private:
      QObject* m_geoView{nullptr};
      GenericListModel* m_levels{nullptr};
      GenericListModel* m_facilities{nullptr};
      GenericListModel* m_sites{nullptr};
      QString m_selectedFacilityId;
      QString m_selectedLevelId;
      QString m_selectedSiteId;
      bool m_selectedSiteRespected{true};
      UpdateLevelsMode m_updatelevelMode{UpdateLevelsMode::AllLevelsMatchingVerticalOrder};
      AutomaticSelectionMode m_automaticSelectionMode{AutomaticSelectionMode::Always};
      bool m_settingViewpoint{false};
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTERCONTROLLER_H
