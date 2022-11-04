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
#include "UtilityNetworkTraceStartingPoint.h"

// ArcGISRuntime headers
#include <Envelope.h>
#include <GeometryEngine.h>
#include <Graphic.h>
#include <Point.h>
#include <Polyline.h>
#include <UtilityAssetGroup.h>
#include <UtilityAssetType.h>
#include <UtilityElement.h>
#include <UtilityNetworkSource.h>
#include <UtilityNetworkTypes.h>
#include <UtilityTerminal.h>
#include <UtilityTerminalConfiguration.h>

namespace Esri::ArcGISRuntime::Toolkit {

UtilityNetworkTraceStartingPoint::UtilityNetworkTraceStartingPoint(QObject* parent) :
  QObject(parent)
{
  //
}

UtilityNetworkTraceStartingPoint::UtilityNetworkTraceStartingPoint(UtilityElement* utilityElement,
                                                                   Graphic* selectionGraphic,
                                                                   Symbol* featureSymbol,
                                                                   Envelope extent,
                                                                   QObject* parent) :
  QObject(parent),
  m_utilityElement(utilityElement),
  m_selectionGraphic(selectionGraphic),
  m_featureSymbol(featureSymbol),
  m_extent(extent)
{
  if (m_utilityElement->assetType()->terminalConfiguration() != nullptr &&
      m_utilityElement->assetType()->terminalConfiguration()->terminals().size() > 1)
  {
    m_hasMultipleTerminals = true;

    m_multipleTerminals = m_utilityElement->assetType()->terminalConfiguration()->terminals();

    QStringList multipleTerminalsTemp{};

    for (const auto& terminal : m_multipleTerminals)
    {
      multipleTerminalsTemp.append(terminal->name());
    }

    m_multipleTerminalNames = multipleTerminalsTemp;
  }

  qDebug() << "@@@ UtilityNetworkTraceStartingPoint c'tor";
  qDebug() << "@@@ m_utilityElement->networkSource()->sourceType()" << static_cast<int>(m_utilityElement->networkSource()->sourceType());
  qDebug() << "@@@ m_selectionGraphic->geometry().geometryType()" << static_cast<int>(m_selectionGraphic->geometry().geometryType());

  if (m_selectionGraphic != nullptr &&
      m_utilityElement->networkSource()->sourceType() == UtilityNetworkSourceType::Edge &&
      m_selectionGraphic->geometry().geometryType() == GeometryType::Polyline)
  {
    m_hasFractionAlongEdge = true;
    setFractionAlongEdge(m_utilityElement->fractionAlongEdge());

    qDebug() << "fraction along edge initial: " << m_utilityElement->fractionAlongEdge();
    const auto polyline = static_cast<Polyline>(m_selectionGraphic->geometry());
    m_selectionGraphic->setGeometry(
          GeometryEngine::createPointAlong(polyline, GeometryEngine::length(polyline) * fractionAlongEdge()));

    connect(this,
            &UtilityNetworkTraceStartingPoint::fractionAlongEdgeChanged,
            this,
            [this, polyline](double newValue, double /*oldValue*/)
    {
      m_selectionGraphic->setGeometry(
            GeometryEngine::createPointAlong(polyline, GeometryEngine::length(polyline) * fractionAlongEdge()));
      m_utilityElement->setFractionAlongEdge(newValue);
    });
  }
}

UtilityNetworkTraceStartingPoint::~UtilityNetworkTraceStartingPoint() = default;

Graphic* UtilityNetworkTraceStartingPoint::selectionGraphic() const
{
  return m_selectionGraphic;
}

UtilityElement* UtilityNetworkTraceStartingPoint::utilityElement() const
{
  return m_utilityElement;
}

Symbol* UtilityNetworkTraceStartingPoint::featureSymbol() const
{
  return m_featureSymbol;
}

bool UtilityNetworkTraceStartingPoint::hasMultipleTerminals() const
{
  return m_hasMultipleTerminals;
}

QStringList UtilityNetworkTraceStartingPoint::multipleTerminalNames() const
{
  return m_multipleTerminalNames;
}

void UtilityNetworkTraceStartingPoint::setMultipleTerminalNames(const QStringList& multipleTerminalNames)
{
  if (m_multipleTerminalNames == multipleTerminalNames)
    return;

  m_multipleTerminalNames = multipleTerminalNames;
  emit multipleTerminalNamesChanged();
}

int UtilityNetworkTraceStartingPoint::selectedTerminalIndex() const
{
  return m_selectedTerminalIndex;
}

void UtilityNetworkTraceStartingPoint::setSelectedTerminalNameByIndex(int index)
{
  if (m_multipleTerminalNames.size() > index)
  {
    m_selectedMultipleTerminal = m_multipleTerminals.at(index);
    m_selectedTerminalIndex = index;
    qDebug() << "&&&&&" << m_selectedMultipleTerminal->name();
  }
}

bool UtilityNetworkTraceStartingPoint::hasFractionAlongEdge() const
{
  return m_hasFractionAlongEdge;
}

double UtilityNetworkTraceStartingPoint::fractionAlongEdge() const
{
  return m_fractionAlongEdge;
}

void UtilityNetworkTraceStartingPoint::setFractionAlongEdge(double fractionAlongEdge)
{
  if (m_fractionAlongEdge == fractionAlongEdge)
    return;

  const auto oldValue = m_fractionAlongEdge;
  m_fractionAlongEdge = fractionAlongEdge;
  emit fractionAlongEdgeChanged(m_fractionAlongEdge, oldValue);
}

Envelope UtilityNetworkTraceStartingPoint::extent() const
{
  return m_extent;
}

QString UtilityNetworkTraceStartingPoint::groupName() const
{
  return utilityElement()->assetGroup()->name();
}

QString UtilityNetworkTraceStartingPoint::sourceName() const
{
  return utilityElement()->networkSource()->name();
}

} // Esri::ArcGISRuntime::Toolkit
