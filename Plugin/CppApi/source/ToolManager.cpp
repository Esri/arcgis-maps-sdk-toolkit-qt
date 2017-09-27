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
#include "ToolResourceProvider.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

ToolManager& ToolManager::instance()
{
  static ToolManager instance;

  return instance;
}

ToolManager::ToolManager():
  m_resourceProvider(new ToolResourceProvider)
{

}

ToolManager::~ToolManager()
{
  delete m_resourceProvider;
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

ToolManager::ToolsList::iterator ToolManager::begin()
{
  return m_tools.begin();
}

ToolManager::ToolsList::iterator ToolManager::end()
{
  return m_tools.end();
}

ToolManager::ToolsList::const_iterator ToolManager::begin() const
{
  return m_tools.cbegin();
}

ToolManager::ToolsList::const_iterator ToolManager::end() const
{
  return m_tools.cend();
}

ToolResourceProvider* ToolManager::resourceProvider() const
{
  return m_resourceProvider;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
