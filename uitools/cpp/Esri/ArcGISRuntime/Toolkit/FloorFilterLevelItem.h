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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFLOORITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFLOORITEM_H

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class FloorLevel;

  namespace Toolkit {

    class FloorFilterLevelItem : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(QString modelId READ modelId NOTIFY floorLevelChanged)
      Q_PROPERTY(QString shortName READ shortName NOTIFY floorLevelChanged)
      Q_PROPERTY(QString longName READ longName NOTIFY floorLevelChanged)
    public:
      Q_INVOKABLE explicit FloorFilterLevelItem(QObject* parent = nullptr);
      Q_INVOKABLE explicit FloorFilterLevelItem(FloorLevel* floorLevel, QObject* parent = nullptr);
      ~FloorFilterLevelItem() override;

      void setFloorLevel(FloorLevel* floorLevel);
      FloorLevel* floorLevel() const;

      QString modelId() const;
      QString shortName() const;
      QString longName() const;

    signals:
      void floorLevelChanged();

    private:
      FloorLevel* m_floorLevel = nullptr;
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFLOORITEM_H
