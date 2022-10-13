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
#include "UtilityNetworkTraceOperationResult.h"

// ArcGISRuntime headers
#include <Graphic.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SymbolTypes.h>
#include <UtilityTraceFunctionOutput.h>
#include <UtilityTraceParameters.h>

namespace Esri::ArcGISRuntime::Toolkit {

UtilityNetworkTraceOperationResult::UtilityNetworkTraceOperationResult(QObject* parent) :
  QObject(parent)
{

}

UtilityNetworkTraceOperationResult::UtilityNetworkTraceOperationResult(UtilityTraceParameters* traceParameters,
                                                                       UtilityNamedTraceConfiguration* traceConfiguration,
                                                                       QObject* parent) :
  QObject(parent),
  m_traceParameters(traceParameters),
  m_traceConfiguration(traceConfiguration)
{
  //
}

UtilityNetworkTraceOperationResult::~UtilityNetworkTraceOperationResult() = default;

UtilityTraceParameters* UtilityNetworkTraceOperationResult::traceParameters() const
{
  return m_traceParameters;
}

void UtilityNetworkTraceOperationResult::setTraceParameters(UtilityTraceParameters* traceParameters)
{
  if (m_traceParameters == traceParameters)
    return;

  m_traceParameters = traceParameters;
  emit traceParametersChanged();
}

UtilityTraceResultListModel* UtilityNetworkTraceOperationResult::rawResults() const
{
  return m_rawResults;
}

void UtilityNetworkTraceOperationResult::setRawResults(UtilityTraceResultListModel* rawResults)
{
  if (m_rawResults == rawResults)
    return;

  m_rawResults = rawResults;
  emit rawResultsChanged();
}

QList<UtilityTraceFunctionOutput*> UtilityNetworkTraceOperationResult::functionResults() const
{
  return m_functionResults;
}

void UtilityNetworkTraceOperationResult::setFunctionResults(QList<UtilityTraceFunctionOutput*> functionResults)
{
  if (m_functionResults == functionResults)
    return;

  m_functionResults = functionResults;
  emit functionResultsChanged();
}

QList<UtilityElement*> UtilityNetworkTraceOperationResult::elementResults() const
{
  return m_elementResults;
}

void UtilityNetworkTraceOperationResult::setElementResults(QList<UtilityElement*> elementResults)
{
  if (m_elementResults == elementResults)
    return;

  m_elementResults = elementResults;
  emit elementResultsChanged();
}

QHash<UtilityAssetGroup*, QList<UtilityElement*>> UtilityNetworkTraceOperationResult::elementResultsByAssetGroup() const
{
  return m_elementResultsByAssetGroup;
}

void UtilityNetworkTraceOperationResult::setElementResultsByAssetGroup(QHash<UtilityAssetGroup*, QList<UtilityElement*>> elementResultsByAssetGroup)
{
  if (m_elementResultsByAssetGroup == elementResultsByAssetGroup)
    return;

  m_elementResultsByAssetGroup = elementResultsByAssetGroup;
  emit elementResultsByAssetGroupChanged();
}

QList<ArcGISFeature*> UtilityNetworkTraceOperationResult::features() const
{
  return m_features;
}

void UtilityNetworkTraceOperationResult::setFeatures(QList<ArcGISFeature*> features)
{
  if (m_features == features)
    return;

  m_features = features;
  emit featuresChanged();
}

QList<QString> UtilityNetworkTraceOperationResult::warnings() const
{
  return m_warnings;
}

void UtilityNetworkTraceOperationResult::setWarnings(QList<QString> warnings)
{
  if (m_warnings == warnings)
    return;

  m_warnings = warnings;
  emit warningsChanged();
}

QList<Graphic*> UtilityNetworkTraceOperationResult::graphics() const
{
  return m_graphics;
}

void UtilityNetworkTraceOperationResult::setGraphics(QList<Graphic*> graphics)
{
  if (m_graphics == graphics)
    return;

  m_graphics = graphics;
  emit graphicsChanged();
}

GraphicsOverlay* UtilityNetworkTraceOperationResult::resultsGraphicsOverlay() const
{
  return m_resultsGraphicsOverlay;
}

void UtilityNetworkTraceOperationResult::setResultsGraphicsOverlay(GraphicsOverlay* resultsGraphicsOverlay)
{
  if (m_resultsGraphicsOverlay == resultsGraphicsOverlay)
    return;

  m_resultsGraphicsOverlay = resultsGraphicsOverlay;
  emit resultsGraphicsOverlayChanged();
}

QString UtilityNetworkTraceOperationResult::name() const
{
  return m_name;
}

void UtilityNetworkTraceOperationResult::setName(QString name)
{
  if (m_name == name)
    return;

  m_name = name;
  emit nameChanged();
}

UtilityNamedTraceConfiguration* UtilityNetworkTraceOperationResult::traceConfiguration() const
{
  return m_traceConfiguration;
}

void UtilityNetworkTraceOperationResult::setTraceConfiguration(UtilityNamedTraceConfiguration* traceConfiguration)
{
  if (m_traceConfiguration == traceConfiguration)
    return;

  m_traceConfiguration = traceConfiguration;
  emit traceConfigurationChanged();
}

bool UtilityNetworkTraceOperationResult::areFeaturesSelected() const
{
  return m_areFeaturesSelected;
}

void UtilityNetworkTraceOperationResult::setAreFeaturesSelected(bool areFeaturesSelected)
{
  if (m_areFeaturesSelected == areFeaturesSelected)
    return;

  m_areFeaturesSelected = areFeaturesSelected;
  emit areFeaturesSelectedChanged();
}

void UtilityNetworkTraceOperationResult::setVisualizationColorProperty(QColor visualizationColorProperty)
{
  for (const auto& graphic : m_graphics)
  {
    const auto symbol = graphic->symbol();
    const auto type = symbol->symbolType();

    if (type == SymbolType::SimpleMarkerSymbol)
    {
      const auto marker = dynamic_cast<SimpleMarkerSymbol*>(symbol);
      Q_ASSERT(marker);
      marker->setColor(visualizationColorProperty);
    }
    else if (type == SymbolType::SimpleLineSymbol)
    {
      const auto line = dynamic_cast<SimpleLineSymbol*>(symbol);
      Q_ASSERT(line);
      line->setColor(visualizationColorProperty);
    }
    else if (type == SymbolType::SimpleFillSymbol)
    {
      const auto fill = dynamic_cast<SimpleFillSymbol*>(symbol);
      Q_ASSERT(fill);

      fill->setColor(visualizationColorProperty);
      if (fill->outline()->symbolType() == SymbolType::SimpleLineSymbol)
      {
        const auto outline = dynamic_cast<SimpleLineSymbol*>(fill->outline());
        Q_ASSERT(outline);
        outline->setColor(visualizationColorProperty);
      }
    }
  }
}

} // // Esri::ArcGISRuntime::Toolkit
