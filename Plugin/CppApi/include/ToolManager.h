// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

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

class TOOLKIT_EXPORT ToolManager {

  using ToolsList = QMap<QString, AbstractTool*>;

public:

  static ToolManager& instance(); // singleton

  ~ToolManager();

  void addTool(AbstractTool* tool);

  AbstractTool* tool(const QString& toolName) const;

  template<class T>
  T* tool() const;

  ToolsList::iterator begin();
  ToolsList::iterator end();

  ToolsList::const_iterator begin() const;
  ToolsList::const_iterator end() const;

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
