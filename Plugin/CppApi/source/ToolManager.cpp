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
  emit toolAdded(tool);
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
      emit toolRemoved(it.key());
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

/*!
  \fn void ToolManager::toolAdded(Esri::ArcGISRuntime::Toolkit::AbstractTool* tool);
  \brief The signal emitted when a tool has been added to the ToolManager.

  \list
  \li \a tool - The tool that was added.
  \endlist
 */

/*!
  \fn void ToolManager::toolRemoved(const QString& toolName);
  \brief The signal emitted when a tool has been removed from the ToolManager.

  \list
  \li \a toolName - The name of the tool that was removed.
  \endlist

  The name of the tool is provided instead of a pointer to the tool
  like with \l toolAdded. This is for safety since the pointer may
  be invalid when the tool has been removed.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
