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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_SMARTLOCATORSEARCHSOURCE_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_SMARTLOCATORSEARCHSOURCE_H

// Toolkit headers
#include "LocatorSearchSource.h"

// ArcGISRuntime headers
#include <LocatorTask.h>
#include <SymbolStyle.h>

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class SmartLocatorSearchSource : public LocatorSearchSource
  {
    Q_OBJECT
  public:
    Q_INVOKABLE SmartLocatorSearchSource(LocatorTask* locatorTask, QObject* parent = nullptr);

    ~SmartLocatorSearchSource() override;

    int repeatSearchResultThreshold() const;
    void setRepeatSearchResultThreshold(int repeatSearchResultThreshold);

    int repeatSuggestResultThreshold() const;
    void setRepeatSuggestResultThreshold(int repeatSearchResultThreshold);

    void search(const SuggestResult& suggestion, const Geometry area = Geometry{}) override;

    void search(const QString& searchString, const Geometry area = Geometry{}) override;

  signals:
    void repeatSuggestResultThresholdChanged();
    void repeatSearchResultThresholdChanged();

  private:
    int m_repeatSearchResultThreshold;
     int m_repeatSearchSuggestThreshold;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_SMARTLOCATORSEARCHSOURCE_H
