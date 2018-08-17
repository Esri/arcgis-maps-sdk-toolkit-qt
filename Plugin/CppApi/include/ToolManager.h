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

#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include "ToolkitCommon.h"

#include <QObject>
#include <QMap>
#include <memory>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class AbstractTool;

class TOOLKIT_EXPORT ToolManager : public QObject
{
  Q_OBJECT

  using ToolsList = QMap<QString, AbstractTool*>;

public:

  static ToolManager& instance(); // singleton

  ~ToolManager();

  void addTool(AbstractTool* tool);

  void removeTool(const QString& toolName);

  void removeTool(AbstractTool* tool);

  void clearTools();

  AbstractTool* tool(const QString& toolName) const;

  template<class T>
  T* tool() const;

  ToolsList::iterator begin();
  ToolsList::iterator end();

  ToolsList::const_iterator begin() const;
  ToolsList::const_iterator end() const;

signals:
  void toolAdded(Esri::ArcGISRuntime::Toolkit::AbstractTool* tool);
  void toolRemoved(const QString& toolName);

private:
  ToolManager();

  ToolsList m_tools;
};

template<class T>
T* ToolManager::tool() const
{
  auto it = begin();
  const auto itEnd = end();
  for (; it != itEnd; ++it)
  {
    AbstractTool* absTool = it.value();
    if (!absTool)
      continue;

    T* tool = qobject_cast<T*>(absTool);
    if (tool)
      return tool;
  }

  return nullptr;
}

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TOOL_MANAGER_H
