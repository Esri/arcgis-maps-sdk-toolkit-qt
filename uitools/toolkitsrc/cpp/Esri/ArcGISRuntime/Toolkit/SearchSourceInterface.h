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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHSOURCEINTERFACE_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHSOURCEINTERFACE_H

// ArcGISRuntime headers
#include <Geometry.h>
#include <Point.h>
#include <TaskWatcher.h>

// Qt headers
#include <QList>
#include <QObject>
#include <QString>
#include <QQmlEngine>

Q_MOC_INCLUDE("SearchResult.h")

// Forward declarations
namespace Esri::ArcGISRuntime {
  class SuggestListModel;
  class SuggestResult;

  namespace Toolkit {
    class SearchResult;
  }
} // Esri::ArcGISRuntime

namespace Esri::ArcGISRuntime::Toolkit {

  class SearchSourceInterface : public QObject
  {
    Q_OBJECT
    QML_INTERFACE
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)

  public:
    SearchSourceInterface(QObject* parent = nullptr);
    ~SearchSourceInterface() override;

    QString displayName() const;
    void setDisplayName(QString displayName);

    virtual int maximumResults() const = 0;
    virtual void setMaximumResults(int maximumResults) = 0;

    virtual int maximumSuggestions() const = 0;
    virtual void setMaximumSuggestions(int maximumSuggestions) = 0;

    virtual Point preferredSearchLocation() const = 0;
    virtual void setPreferredSearchLocation(Point preferredSearchLocation) = 0;

    virtual SuggestListModel* suggestions() const = 0;

    virtual void search(const SuggestResult& suggestion, Geometry area = Geometry{}) = 0;

    virtual void search(const QString& query, Geometry area = Geometry{}) = 0;

  signals:
    void displayNameChanged();

    void searchCompleted(QList<Esri::ArcGISRuntime::Toolkit::SearchResult*> searchResults);

  private:
    QString m_displayName;
  };

} // Esri::ArcGISRuntime::Toolkit

Q_DECLARE_INTERFACE(Esri::ArcGISRuntime::Toolkit::SearchSourceInterface, "Esri::ArcGISRuntime::Toolkit::SearchSourceInterface")

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_SEARCHSOURCEINTERFACE_H
