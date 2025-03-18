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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACESTARTINGPOINT_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACESTARTINGPOINT_H

// Qt headers
#include <QObject>

// STL headers
#include <Envelope.h>
#include <GeoView.h>
#include <UtilityNetwork.h>

// Other headers
#include "GenericListModel.h"

namespace Esri::ArcGISRuntime {
class Graphic;
class Symbol;
} // Esri::ArcGISRuntime

namespace Esri::ArcGISRuntime::Toolkit {

class UtilityNetworkTraceStartingPoint : public QObject
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit UtilityNetworkTraceStartingPoint(QObject* parent = nullptr);
  Q_INVOKABLE UtilityNetworkTraceStartingPoint(Esri::ArcGISRuntime::UtilityElement* utilityElement,
                                               Esri::ArcGISRuntime::Graphic* selectionGraphic,
                                               Esri::ArcGISRuntime::Symbol* featureSymbol,
                                               Esri::ArcGISRuntime::Envelope extent,
                                               QObject* parent = nullptr);
  ~UtilityNetworkTraceStartingPoint() override;

  Graphic* selectionGraphic() const;

  UtilityElement* utilityElement() const;

  Symbol* featureSymbol() const;

  bool hasMultipleTerminals() const;

  QStringList multipleTerminalNames() const;
  void setMultipleTerminalNames(const QStringList& multipleTerminals);
  int selectedTerminalIndex() const;
  void setSelectedTerminalNameByIndex(int index);

  bool hasFractionAlongEdge() const;

  double fractionAlongEdge() const;
  void setFractionAlongEdge(double fractionAlongEdge);

  Envelope extent() const;

  QString groupName() const;
  QString sourceName() const;

signals:
  void multipleTerminalNamesChanged();
  void fractionAlongEdgeChanged(double newValue, double oldValue);

private:
  UtilityElement* m_utilityElement = nullptr;
  Graphic* m_selectionGraphic = nullptr;
  Symbol* m_featureSymbol = nullptr;
  Envelope m_extent;
  bool m_hasMultipleTerminals = false;
  QStringList m_multipleTerminalNames;
  QList<UtilityTerminal*> m_multipleTerminals;
  UtilityTerminal* m_selectedMultipleTerminal = nullptr;
  int m_selectedTerminalIndex = 0;
  bool m_hasFractionAlongEdge = false;
  double m_fractionAlongEdge = 0.0;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACESTARTINGPOINT_H
