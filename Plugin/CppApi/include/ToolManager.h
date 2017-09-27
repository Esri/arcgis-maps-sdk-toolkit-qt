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
class ToolResourceProvider;

class TOOLKIT_EXPORT ToolManager {

  typedef QMap<QString, AbstractTool*> ToolsList;

public:

  static std::shared_ptr<ToolManager> instance(); // singleton

  ToolManager();
  ~ToolManager();

  void addTool(AbstractTool* tool);

  AbstractTool* tool(const QString& toolName) const;
  ToolsList::iterator toolsBegin();
  ToolsList::iterator toolsEnd();

  ToolResourceProvider* resourceProvider() const;

private:
  ToolsList m_tools;
  ToolResourceProvider* m_resourceProvider;

  static std::shared_ptr<ToolManager> m_instance;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TOOL_MANAGER_H
