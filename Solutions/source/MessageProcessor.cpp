/*******************************************************************************
 * Copyright 2012-2017 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

#include "MessageProcessor.h"
#include "DictionarySymbolStyle.h"
#include "GeoView.h"
#include "GraphicsOverlay.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Solutions {

MessageProcessor::MessageProcessor(DictionarySymbolStyle* dictionarySymbolStyle, GeoView* geoView, QObject* parent) :
  QObject(parent),
  m_geoView(geoView),
  m_dictionarySymbolStyle(dictionarySymbolStyle)
{
}

MessageProcessor::~MessageProcessor()
{
}

GeoView* MessageProcessor::geoView() const
{
  return m_geoView;
}

QList<GraphicsOverlay*> MessageProcessor::graphicsOverlays() const
{
  return m_graphicsOverlays;
}

DictionarySymbolStyle* MessageProcessor::dictionarySymbolStyle() const
{
  return m_dictionarySymbolStyle;
}

void MessageProcessor::setDictionarySymbolStyle(DictionarySymbolStyle* dictionarySymbolStyle)
{
  Q_UNIMPLEMENTED();
  Q_UNUSED(dictionarySymbolStyle)
}

bool MessageProcessor::processMessage(const Message& message)
{
  Q_UNIMPLEMENTED();
  Q_UNUSED(message)
  return false;
}

} // namespace Solutions
} // namespace ArcGISRuntime
} // namespace Esri
