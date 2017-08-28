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

#include "ToolManager.h"
#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Solutions
{

std::shared_ptr<ToolManager> ToolManager::m_instance;

std::shared_ptr<ToolManager> ToolManager::instance()
{
  if (m_instance == nullptr)
    m_instance = std::shared_ptr<ToolManager>(new ToolManager);

  return m_instance;
}

void ToolManager::addTool(AbstractTool* tool)
{
  if (!tool)
    return;

  m_tools.insert(tool->toolName(), tool);
}

AbstractTool* ToolManager::tool(const QString& toolName) const
{
  return m_tools[toolName];
}

ToolManager::ToolsList::iterator ToolManager::toolsBegin()
{
  return m_tools.begin();
}

ToolManager::ToolsList::iterator ToolManager::toolsEnd()
{
  return m_tools.end();
}

} // Solutions
} // ArcGISRuntime
} // Esri
