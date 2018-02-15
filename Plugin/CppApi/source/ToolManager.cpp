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

/*! \brief Returns the singleton instance of the tool manager.
 *
 */
ToolManager& ToolManager::instance()
{
  static ToolManager instance;

  return instance;
}

/*! \internal.
 *
 */
ToolManager::ToolManager()
{

}

/*! \brief Destructor.
 *
 */
ToolManager::~ToolManager()
{

}

/*! \brief Adds \a tool to the manager.
 *
 * The \l AbstractTool can be retrieved from the manager by supplying
 * the tool name or by requesting it by type.
 */
void ToolManager::addTool(AbstractTool* tool)
{
  if (!tool)
    return;

  // when a tool is destroyed, remove it from the manager
  QObject::connect(tool, &AbstractTool::destroyed, tool, [this, tool]()
  {
    removeTool(tool);
  });

  m_tools.insert(tool->toolName(), tool);
}

/*! \brief Removes the \l AbstractTool called \a toolName from the manager.
 */
void ToolManager::removeTool(const QString& toolName)
{
  m_tools.remove(toolName);
}

/*! \brief Removes the \l AbstractTool \a tool from the manager.
 */
void ToolManager::removeTool(AbstractTool* tool)
{
  if (tool == nullptr)
    return;

  auto it = m_tools.begin();
  auto itEnd = m_tools.end();
  for(; it != itEnd; ++it)
  {
    if (it.value() == tool)
    {
      m_tools.erase(it);
      return;
    }
  }
}

/*! \brief Clears all tools from the manager.
 */
void ToolManager::clearTools()
{
  m_tools.clear();
}

/*! \brief Retrieve the \l AbsgtractTool with the name \a toolName.
 *
 * return \c nullptr if the tool cannot be found.
 */
AbstractTool* ToolManager::tool(const QString& toolName) const
{
  return m_tools[toolName];
}

/*! \brief Returns a begin iterator to the list of tools.
 *
 */
ToolManager::ToolsList::iterator ToolManager::begin()
{
  return m_tools.begin();
}

/*! \brief Returns an end iterator to the list of tools.
 *
 */
ToolManager::ToolsList::iterator ToolManager::end()
{
  return m_tools.end();
}

/*! \brief Returns a begin (const) iterator to the list of tools.
 *
 */
ToolManager::ToolsList::const_iterator ToolManager::begin() const
{
  return m_tools.cbegin();
}

/*! \brief Returns an end (const) iterator to the list of tools.
 *
 */
ToolManager::ToolsList::const_iterator ToolManager::end() const
{
  return m_tools.cend();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
