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

#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

AbstractTool::AbstractTool(QObject* parent /*= nullptr*/):
  QObject(parent)
{

}

AbstractTool::~AbstractTool()
{

}

bool AbstractTool::handleClick(const Point& pos)
{
  Q_UNUSED(pos)
  return false;
}

void AbstractTool::setActive(bool active)
{
  m_active = active;
}

bool AbstractTool::isActive() const
{
  return m_active;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
