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

#ifndef QRT_TOOLKIT_MessageProcessor_H
#define QRT_TOOLKIT_MessageProcessor_H

#include <QObject>
#include "Message.h"

namespace Esri {
namespace ArcGISRuntime {

class GeoView;
class DictionarySymbolStyle;
class GraphicsOverlay;

namespace Toolkit {

class TOOLKIT_EXPORT MessageProcessor : public QObject
{
public:
  MessageProcessor() = delete;
  MessageProcessor(DictionarySymbolStyle* dictionarySymbolStyle, GeoView* geoView, QObject* parent = nullptr);
  ~MessageProcessor();

  GeoView* geoView() const;

  QList<GraphicsOverlay*> graphicsOverlays() const;

  DictionarySymbolStyle* dictionarySymbolStyle() const;
  void setDictionarySymbolStyle(DictionarySymbolStyle* dictionarySymbolStyle);

  bool processMessage(const Message& message);

private:
  Q_DISABLE_COPY(MessageProcessor)
  GeoView* m_geoView = nullptr;
  QList<GraphicsOverlay*> m_graphicsOverlays;
  DictionarySymbolStyle* m_dictionarySymbolStyle = nullptr;
};

} // namespace Toolkit
} // namespace ArcGISRuntime
} // namespace Esri

#endif // QRT_TOOLKIT_MessageProcessor_H
