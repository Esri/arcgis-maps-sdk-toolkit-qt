/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include "ToolkitCommon.h"

#include <QObject>
#include <QString>
#include <QVariantMap>

namespace Esri
{
namespace ArcGISRuntime
{

class Error;
class Point;

namespace Toolkit
{

class TOOLKIT_EXPORT AbstractTool : public QObject
{
  Q_OBJECT

  /*! \internal */
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
  AbstractTool(QObject* parent = nullptr);
  ~AbstractTool() override;

  virtual QString toolName() const = 0;
  virtual bool handleClick(const Point& pos);

  virtual void setProperties(const QVariantMap& properties);

  virtual void setActive(bool active);
  bool isActive() const;

signals:
  void activeChanged();
  void errorOccurred(const Error& error);
  void propertyChanged(const QString& propertyName, const QVariant& propertyValue);

protected:
  bool m_active = false;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ABSTRACT_TOOL_H
