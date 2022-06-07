/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
