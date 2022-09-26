/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHRESULT_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHRESULT_H

// Toolkit headers
#include "SearchSourceInterface.h"

// ArcGISRuntime headers
#include "GeoElement.h"
#include "Viewpoint.h"

// Qt headers
#include <QObject>
#include <QString>
#include <QUrl>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class SearchResult : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QString displayTitle READ displayTitle WRITE setDisplayTitle NOTIFY displayTitleChanged)
    Q_PROPERTY(QString displaySubtitle READ displaySubtitle WRITE setDisplaySubtitle NOTIFY displaySubtitleChanged)
    Q_PROPERTY(QUrl markerImageUrl READ markerImageUrl WRITE setMarkerImageUrl NOTIFY markerImageUrlChanged)
    Q_PROPERTY(SearchSourceInterface* owningSource READ owningSource WRITE setOwningSource NOTIFY owningSourceChanged)
  public:
    Q_INVOKABLE SearchResult(QObject* parent = nullptr);

    ~SearchResult() override;

    QString displayTitle() const;
    void setDisplayTitle(QString displayTitle);

    QString displaySubtitle() const;
    void setDisplaySubtitle(QString displaySubtitle);

    QUrl markerImageUrl() const;
    void setMarkerImageUrl(QUrl markerImageUrl);

    SearchSourceInterface* owningSource();
    void setOwningSource(SearchSourceInterface* owningSource);

    GeoElement* geoElement() const;
    void setGeoElement(GeoElement* geoElement);

    Viewpoint selectionViewpoint() const;
    void setSelectionViewpoint(Viewpoint selectionViewpoint);

  signals:
    void displayTitleChanged();
    void displaySubtitleChanged();
    void markerImageUrlChanged();
    void owningSourceChanged();
    void geoElementChanged();
    void selectionViewpointChanged();

  private:
    QString m_displayTitle;
    QString m_displaySubtitle;
    QUrl m_markerImageUrl;
    SearchSourceInterface* m_owningSource = nullptr;
    GeoElement* m_geoElement = nullptr;
    Viewpoint m_selectionViewpoint;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHRESULT_H
