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

#ifndef QRT_SOLUTIONS_Message_H
#define QRT_SOLUTIONS_Message_H

#include <QHash>
#include <QString>
#include "Common.h"
#include "Geometry.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Solutions {

class SOLUTIONS_EXPORT Message
{
public:
  Message();
  Message(Message& other);
  Message(Message&& other);
  ~Message();

  Message& operator=(Message& other);
  Message& operator=(Message&& other);

  QString symbolId() const;
  void setSymbolId(const QString& symbolId);

  Geometry geometry() const;
  void setGeometry(const Geometry& geometry);

  QString action() const;
  void setAction(const QString& action);

  QString messageId() const;
  void setMessageId(const QString& messageId);

  QString messageType() const;
  void setMessageType(const QString& messageType);

  QString symbolJson() const;
  void setSymbolJson(const QString& symbolJson);

  QHash<QString, QString> attributes() const;
  void setAttributes(const QHash<QString, QString>& attributes);

private:
  QString m_symbolId;
  QString m_action;
  QString m_messageId;
  QString m_messageType;
  QString m_symbolJson;
  QHash<QString, QString> m_attributes;
  Geometry m_geometry;
};

} // namespace Solutions
} // namespace ArcGISRuntime
} // namespace Esri

#endif // QRT_SOLUTIONS_Message_H
