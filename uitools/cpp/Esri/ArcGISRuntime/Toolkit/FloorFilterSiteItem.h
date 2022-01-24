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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERSITEITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERSITEITEM_H

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class FloorSite;

  namespace Toolkit {
    class FloorFilterSiteItem : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(QString modelId READ modelId NOTIFY floorSiteChanged)
      Q_PROPERTY(QString name READ name NOTIFY floorSiteChanged)
    public:
      Q_INVOKABLE explicit FloorFilterSiteItem(QObject* parent = nullptr);
      Q_INVOKABLE explicit FloorFilterSiteItem(FloorSite* floorSite, QObject* parent = nullptr);
      ~FloorFilterSiteItem() override;

      void setFloorSite(FloorSite* floorSite);
      FloorSite* floorSite() const;

      QString modelId() const;
      QString name() const;

    signals:
      void floorSiteChanged();

    private:
      FloorSite* m_floorSite = nullptr;
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFITLERFSITEITEM_H
