// Copyright 2022 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
#include "FloorFilterDemo.h"

// ArcGISRuntime imports
#include <Map.h>
#include <PortalItem.h>
#include <Scene.h>

using namespace Esri::ArcGISRuntime;

FloorFilterDemo::FloorFilterDemo(QObject* parent) :
  BaseDemo(parent)
{
}

FloorFilterDemo::~FloorFilterDemo()
{
}

Map* FloorFilterDemo::initMap_(QObject* parent) const
{
  return new Map(new PortalItem("f133a698536f44c8884ad81f80b6cfc7", parent), parent);
}

Scene* FloorFilterDemo::initScene_(QObject* parent) const
{
  return BaseDemo::initScene_(parent);
  // return new Scene("f133a698536f44c8884ad81f80b6cfc7", parent);
}
