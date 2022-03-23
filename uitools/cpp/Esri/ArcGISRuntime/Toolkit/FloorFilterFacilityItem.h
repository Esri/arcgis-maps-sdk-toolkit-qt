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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFACILITYITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFACILITYITEM_H

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class FloorFacility;

  namespace Toolkit {
    class FloorFilterFacilityItem : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(QString modelId READ modelId NOTIFY floorFacilityChanged)
      Q_PROPERTY(QString name READ name NOTIFY floorFacilityChanged)
      Q_PROPERTY(QString parentSiteName READ parentSiteName NOTIFY floorFacilityChanged)
    public:
      Q_INVOKABLE explicit FloorFilterFacilityItem(QObject* parent = nullptr);
      Q_INVOKABLE explicit FloorFilterFacilityItem(FloorFacility* floorFacility, QObject* parent = nullptr);
      ~FloorFilterFacilityItem() override;

      void setFloorFacility(FloorFacility* floorFacility);
      FloorFacility* floorFacility() const;

      QString modelId() const;
      QString name() const;
      QString parentSiteName() const;

    signals:
      void floorFacilityChanged();

    private:
      FloorFacility* m_floorFacility = nullptr;
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFACILITYITEM_H
