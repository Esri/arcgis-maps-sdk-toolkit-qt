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

#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include "SolutionsCommon.h"
#include <QString>

namespace Esri
{
namespace ArcGISRuntime
{

class Point;

namespace Solutions
{

class SOLUTIONS_EXPORT AbstractTool
{
public:
  AbstractTool();
  virtual ~AbstractTool();

  virtual QString toolName() const = 0;
  virtual bool handleClick(const Point& pos);

  virtual void setActive(bool active);
  bool isActive() const;

protected:
  bool m_active = false;
};

} // Solutions
} // ArcGISRuntime
} // Esri

#endif // ABSTRACT_TOOL_H
