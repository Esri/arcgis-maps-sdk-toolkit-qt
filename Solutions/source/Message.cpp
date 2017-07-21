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

#include "Message.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Solutions {

Message::Message()
{
}

Message::Message(Message& other) :
  m_symbolId(other.m_symbolId),
  m_action(other.m_action),
  m_messageId(other.m_messageId),
  m_messageType(other.m_messageType),
  m_symbolJson(other.m_symbolJson),
  m_attributes(other.m_attributes),
  m_geometry(other.m_geometry)
{
}

Message::Message(Message&& other) :
  m_symbolId(std::move(other.m_symbolId)),
  m_action(std::move(other.m_action)),
  m_messageId(std::move(other.m_messageId)),
  m_messageType(std::move(other.m_messageType)),
  m_symbolJson(std::move(other.m_symbolJson)),
  m_attributes(std::move(other.m_attributes)),
  m_geometry(std::move(other.m_geometry))
{
}

Message::~Message()
{
}

Message& Message::operator=(Message& other)
{
  if (this != &other)
  {
    m_symbolId = other.m_symbolId;
    m_action = other.m_action;
    m_messageId = other.m_messageId;
    m_messageType = other.m_messageType;
    m_symbolJson = other.m_symbolJson;
    m_attributes = other.m_attributes;
    m_geometry = other.m_geometry;
  }

  return *this;
}

Message& Message::operator=(Message&& other)
{
  if (this != &other)
  {
    m_symbolId = std::move(other.m_symbolId);
    m_action = std::move(other.m_action);
    m_messageId = std::move(other.m_messageId);
    m_messageType = std::move(other.m_messageType);
    m_symbolJson = std::move(other.m_symbolJson);
    m_attributes = std::move(other.m_attributes);
    m_geometry = std::move(other.m_geometry);
  }

  return *this;
}

QString Message::symbolId() const
{
  return m_symbolId;
}

void Message::setSymbolId(const QString& symbolId)
{
  m_symbolId = symbolId;
}

Geometry Message::geometry() const
{
  return m_geometry;
}

void Message::setGeometry(const Geometry& geometry)
{
  m_geometry = geometry;
}

QString Message::action() const
{
  return m_action;
}

void Message::setAction(const QString& action)
{
  m_action = action;
}

QString Message::messageId() const
{
  return m_messageId;
}

void Message::setMessageId(const QString& messageId)
{
  m_messageId = messageId;
}

QString Message::messageType() const
{
  return m_messageType;
}

void Message::setMessageType(const QString& messageType)
{
  m_messageType = messageType;
}

QString Message::symbolJson() const
{
  return m_symbolJson;
}

void Message::setSymbolJson(const QString& symbolJson)
{
  m_symbolJson = symbolJson;
}

QHash<QString, QString> Message::attributes() const
{
  return m_attributes;
}

void Message::setAttributes(const QHash<QString, QString>& attributes)
{
  m_attributes = attributes;
}

} // namespace Solutions
} // namespace ArcGISRuntime
} // namespace Esri
