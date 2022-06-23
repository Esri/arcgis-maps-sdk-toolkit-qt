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

#include "AuthenticationViewDemo.h"

#include "Map.h"
#include "Scene.h"

AuthenticationViewDemo::AuthenticationViewDemo(QObject* parent) :
  BaseDemo(parent)
{
}

AuthenticationViewDemo::~AuthenticationViewDemo()
{
}

Esri::ArcGISRuntime::Map* AuthenticationViewDemo::initMap_(QObject* parent) const
{
  //return a map that requires authentication
  return new Esri::ArcGISRuntime::Map(
      new Esri::ArcGISRuntime::PortalItem(QStringLiteral("e5039444ef3c48b8a8fdc9227f9be7c1"), parent),
      parent);
}

Esri::ArcGISRuntime::Scene* AuthenticationViewDemo::initScene_(QObject* /*parent*/) const
{
  return nullptr;
}
