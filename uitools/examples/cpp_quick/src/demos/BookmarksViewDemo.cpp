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
#include "BookmarksViewDemo.h"

#include "Map.h"
#include "Scene.h"

using namespace Esri::ArcGISRuntime;

BookmarksViewDemo::BookmarksViewDemo(QObject* parent) :
  BaseDemo(parent)
{
}

BookmarksViewDemo::~BookmarksViewDemo() = default;

Map* BookmarksViewDemo::initMap_(QObject* parent) const
{
  auto map = new Map(new PortalItem("16f1b8ba37b44dc3884afc8d5f454dd2", parent), parent);
  return map;
}

Scene* BookmarksViewDemo::initScene_(QObject* /*parent*/) const
{
  return nullptr;
}
