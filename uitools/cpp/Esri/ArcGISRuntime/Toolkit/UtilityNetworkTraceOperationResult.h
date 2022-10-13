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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACEOPERATIONRESULT_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACEOPERATIONRESULT_H

// Toolkit headers
#include "Internal/GenericListModel.h"

// ArcGISRuntime headers
#include <GeoView.h>
#include <UtilityNetwork.h>

// Qt headers
#include <QObject>

Q_MOC_INCLUDE("UtilityTraceParameters.h")
Q_MOC_INCLUDE("UtilityTraceResultListModel.h")
Q_MOC_INCLUDE("UtilityTraceFunctionOutput.h")
Q_MOC_INCLUDE("UtilityElement.h")
Q_MOC_INCLUDE("ArcGISFeature.h")
Q_MOC_INCLUDE("Graphic.h")
Q_MOC_INCLUDE("GraphicsOverlay.h")
Q_MOC_INCLUDE("UtilityNamedTraceConfiguration.h")

namespace Esri::ArcGISRuntime {
  class Graphic;
  class UtilityTraceFunctionOutput;
}

namespace Esri::ArcGISRuntime::Toolkit {

  class UtilityAssetGroup;

  class UtilityNetworkTraceOperationResult : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(UtilityTraceParameters* traceParameters READ traceParameters WRITE setTraceParameters NOTIFY traceParametersChanged)
    Q_PROPERTY(UtilityTraceResultListModel* rawResults READ rawResults WRITE setRawResults NOTIFY rawResultsChanged)
    Q_PROPERTY(QList<UtilityTraceFunctionOutput*> functionResults READ functionResults WRITE setFunctionResults NOTIFY functionResultsChanged)
    Q_PROPERTY(QList<UtilityElement*> elementResults READ elementResults WRITE setElementResults NOTIFY elementResultsChanged)
    Q_PROPERTY(QList<ArcGISFeature*> features READ features WRITE setFeatures NOTIFY featuresChanged)
    Q_PROPERTY(QList<QString> warnings READ warnings WRITE setWarnings NOTIFY warningsChanged)
    Q_PROPERTY(QList<Graphic*> graphics READ graphics WRITE setGraphics NOTIFY graphicsChanged)
    Q_PROPERTY(GraphicsOverlay* resultsGraphicsOverlay READ resultsGraphicsOverlay WRITE setResultsGraphicsOverlay NOTIFY resultsGraphicsOverlayChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(UtilityNamedTraceConfiguration* traceConfiguration READ traceConfiguration WRITE setTraceConfiguration NOTIFY traceConfigurationChanged)
    Q_PROPERTY(bool areFeaturesSelected READ areFeaturesSelected WRITE setAreFeaturesSelected NOTIFY areFeaturesSelectedChanged)

  public:
    Q_INVOKABLE explicit UtilityNetworkTraceOperationResult(QObject* parent = nullptr);
    Q_INVOKABLE UtilityNetworkTraceOperationResult(Esri::ArcGISRuntime::UtilityTraceParameters* traceParameters,
                                                   Esri::ArcGISRuntime::UtilityNamedTraceConfiguration* traceConfiguration,
                                                   QObject* parent = nullptr);
    ~UtilityNetworkTraceOperationResult() override;

    UtilityTraceParameters* traceParameters() const;
    void setTraceParameters(UtilityTraceParameters* traceParameters);

    UtilityTraceResultListModel* rawResults() const;
    void setRawResults(UtilityTraceResultListModel* rawResults);

    QList<UtilityTraceFunctionOutput*> functionResults() const;
    void setFunctionResults(QList<UtilityTraceFunctionOutput*> functionResults);

    QList<UtilityElement*> elementResults() const;
    void setElementResults(QList<UtilityElement*> elementResults);

    QHash<UtilityAssetGroup*, QList<UtilityElement*>> elementResultsByAssetGroup() const;
    void setElementResultsByAssetGroup(QHash<UtilityAssetGroup*, QList<UtilityElement*>> elementResultsByAssetGroup);

    QList<ArcGISFeature*> features() const;
    void setFeatures(QList<ArcGISFeature*> features);

    QList<QString> warnings() const;
    void setWarnings(QList<QString> warnings);

    QList<Graphic*> graphics() const;
    void setGraphics(QList<Graphic*> graphics);

    GraphicsOverlay* resultsGraphicsOverlay() const;
    void setResultsGraphicsOverlay(GraphicsOverlay* resultsGraphicsOverlay);

    QString name() const;
    void setName(QString name);

    UtilityNamedTraceConfiguration* traceConfiguration() const;
    void setTraceConfiguration(UtilityNamedTraceConfiguration* traceConfiguration);

    bool areFeaturesSelected() const;
    void setAreFeaturesSelected(bool areFeaturesSelected);

    //SimpleBooleanProperty* areFeaturesSelectedProperty();

    void setVisualizationColorProperty(QColor visualizationColorProperty);

  signals:
    void traceParametersChanged();
    void rawResultsChanged();
    void functionResultsChanged();
    void elementResultsChanged();
    void elementResultsByAssetGroupChanged();
    void featuresChanged();
    void warningsChanged();
    void graphicsChanged();
    void resultsGraphicsOverlayChanged();
    void nameChanged();
    void traceConfigurationChanged();
    void areFeaturesSelectedChanged();

  private:
    UtilityTraceParameters* m_traceParameters = nullptr;
    UtilityTraceResultListModel* m_rawResults = nullptr;
    QList<UtilityTraceFunctionOutput*> m_functionResults;
    QList<UtilityElement*> m_elementResults{};
    QHash<UtilityAssetGroup*, QList<UtilityElement*>> m_elementResultsByAssetGroup;
    QList<ArcGISFeature*> m_features;
    QList<QString> m_warnings;
    QList<Graphic*> m_graphics;
    GraphicsOverlay* m_resultsGraphicsOverlay = nullptr;
    QString m_name;
    UtilityNamedTraceConfiguration* m_traceConfiguration = nullptr;
    bool m_areFeaturesSelected = true;
  };

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACEOPERATIONRESULT_H
