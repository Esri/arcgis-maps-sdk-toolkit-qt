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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHVIEWCONTROLLER_H

// Toolkit headers
#include "Internal/GenericListModel.h"
#include "SearchResult.h"
#include "SearchSourceInterface.h"
#include "SearchSuggestion.h"

// ArcGISRuntime headers
#include <Geometry.h>

// Qt headers
#include <QAbstractListModel>
#include <QObject>
#include <QPointer>

// Forward declarations
namespace Esri::ArcGISRuntime {
  class Map;
  class Scene;
  class SymbolStyle;
  class Graphic;
  class GraphicsOverlay;
} // Esri::ArcGISRuntime

namespace Esri::ArcGISRuntime::Toolkit {

  class SearchViewController : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
    Q_PROPERTY(QString defaultPlaceholder READ defaultPlaceholder WRITE setDefaultPlaceholder NOTIFY defaultPlaceholderChanged)
    Q_PROPERTY(SearchSourceInterface* activeSource READ activeSource WRITE setActiveSource NOTIFY activeSourceChanged)
    Q_PROPERTY(QString currentQuery READ currentQuery WRITE setCurrentQuery NOTIFY currentQueryChanged)
    Q_PROPERTY(SearchResultMode resultMode READ resultMode WRITE setResultMode NOTIFY resultModeChanged)
    Q_PROPERTY(QAbstractListModel* results READ results CONSTANT)
    Q_PROPERTY(SearchResult* selectedResult READ selectedResult WRITE setSelectedResult NOTIFY selectedResultChanged)
    Q_PROPERTY(QAbstractListModel* sources READ sources CONSTANT)
    Q_PROPERTY(QAbstractListModel* suggestions READ suggestions CONSTANT)
    Q_PROPERTY(bool eligableForRequery READ isEligableForRequery WRITE setIsEligableForRequery NOTIFY isEligableForRequeryChanged)
    Q_PROPERTY(bool automaticConfigurationEnabled READ isAutomaticConfigurationEnabled WRITE setIsAutomaticConfigurationEnabled NOTIFY isAutomaticConfigurationEnabledChanged)
    Q_PROPERTY(double thresholdRatioRepeatSearch READ thresholdRatioRepeatSearch WRITE setThresholdRatioRepeatSearch NOTIFY thresholdRatioRepeatSearchChanged)
  public:
    enum class SearchResultMode
    {
      Single,
      Multiple,
      Automatic
    };
    Q_ENUM(SearchResultMode)

    Q_INVOKABLE explicit SearchViewController(QObject* parent = nullptr);

    ~SearchViewController() override;

    QObject* geoView() const;
    void setGeoView(QObject* geoView);

    QString defaultPlaceholder() const;
    void setDefaultPlaceholder(QString defaultPlaceholder);

    SearchSourceInterface* activeSource() const;
    void setActiveSource(SearchSourceInterface* activeSource);

    QString currentQuery() const;
    void setCurrentQuery(QString currentQuery);

    Geometry queryArea() const;
    void setQueryArea(Geometry queryArea);

    Point queryCenter() const;
    void setQueryCenter(Point queryCenter);

    GenericListModel* sources() const;

    GenericListModel* suggestions() const;

    GenericListModel* results() const;

    SearchResult* selectedResult() const;
    void setSelectedResult(SearchResult* selectedResult);

    SearchResultMode resultMode() const;
    void setResultMode(SearchResultMode resultMode);

    bool isEligableForRequery() const;
    void setIsEligableForRequery(bool isEligableForRequery);

    bool isAutomaticConfigurationEnabled() const;
    void setIsAutomaticConfigurationEnabled(bool isAutomaticConfigurationEnabled);

    double thresholdRatioRepeatSearch();
    void setThresholdRatioRepeatSearch(double);

    Q_INVOKABLE void commitSearch(bool restrictToArea);

    Q_INVOKABLE void acceptSuggestion(SearchSuggestion* searchSuggestion);

    Q_INVOKABLE void clearSearch();

  signals:
    void geoViewChanged();
    void defaultPlaceholderChanged();
    void activeSourceChanged();
    void currentQueryChanged();
    void queryAreaChanged();
    void queryCenterChanged();
    void resultModeChanged();
    void selectedResultChanged();
    void isEligableForRequeryChanged();
    void isAutomaticConfigurationEnabledChanged();
    void thresholdRatioRepeatSearchChanged();

  private:
    void onQueryChanged();
    void onSourcesAdded(const QModelIndex& parent, int firstSource, int lastSource);
    void onSourcesRemoved(const QModelIndex& parent, int firstSource, int lastSource);

    bool checkPanningDifferenceLastSearch(const Geometry& geom) const;
    bool checkZoomingDifferenceLastSearch(const Geometry& geom) const;

  private:
    QObject* m_geoView{nullptr};
    GenericListModel* m_suggestions{nullptr};
    GenericListModel* m_sources{nullptr};
    GenericListModel* m_results{nullptr};
    GraphicsOverlay* m_graphicsOverlay{nullptr};
    QPointer<SearchSourceInterface> m_activeSource;
    QPointer<SearchResult> m_selectedResult;
    QString m_currentQuery;
    QString m_defaultPlaceholder;
    SearchResultMode m_resultMode{SearchResultMode::Automatic};
    bool m_isEligableForRequery{false};
    bool m_isAutomaticConfigurationEnabled{true};
    Point m_queryCenter;
    Geometry m_queryArea;
    Geometry m_lastSearchArea;
    double m_thresholdRatioRepeatSearch{0.25};
  };

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHVIEWCONTROLLER_H
