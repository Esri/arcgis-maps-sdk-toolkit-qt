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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_LOCATORSEARCHSOURCE_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_LOCATORSEARCHSOURCE_H

// Toolkit headers
#include "SearchSourceInterface.h"

// ArcGISRuntime headers
#include <GeocodeParameters.h>
#include <LocatorTask.h>
#include <SuggestParameters.h>

// Qt headers
#include <QFuture>
#include <QList>
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class LocatorSearchSource : public SearchSourceInterface
  {
    Q_OBJECT
  public:
    Q_INVOKABLE LocatorSearchSource(LocatorTask* locatorTask, QObject* parent = nullptr);

    ~LocatorSearchSource() override;

    LocatorTask* locator();

    GeocodeParameters geocodeParameters() const;

    SuggestParameters suggestParameters() const;

  public: // SearchSource Interface overrides.
    int maximumResults() const override;
    void setMaximumResults(int maximumResults) override;

    int maximumSuggestions() const override;
    void setMaximumSuggestions(int maximumSuggestions) override;

    Point preferredSearchLocation() const override;
    void setPreferredSearchLocation(Point preferredSearchLocation) override;

    SuggestListModel* suggestions() const override;

    void search(const SuggestResult& suggestion, const Geometry area = Geometry{}) override;

    void search(const QString& searchString, const Geometry area = Geometry{}) override;

  protected:
    TaskWatcher& searchTask();

    const TaskWatcher& searchTask() const;

  private:
    LocatorTask* m_locatorTask;
    GeocodeParameters m_geocodeParameters;
    TaskWatcher m_searchTask;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_LOCATORSEARCHSOURCE_H
