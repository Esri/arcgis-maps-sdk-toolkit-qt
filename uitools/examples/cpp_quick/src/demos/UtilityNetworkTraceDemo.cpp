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
#include "UtilityNetworkTraceDemo.h"

#include <Map.h>
#include <Credential.h>
#include <Portal.h>
#include <PortalItem.h>
#include <Error.h>
#include <AuthenticationChallenge.h>
#include <AuthenticationManager.h>

using namespace Esri::ArcGISRuntime;

UtilityNetworkTraceDemo::UtilityNetworkTraceDemo(QObject* parent) :
  BaseDemo(parent)
{
  connect(AuthenticationManager::instance(),
          &AuthenticationManager::authenticationChallenge,
          this,
          [parent](AuthenticationChallenge* challenge)
          {
            challenge->continueWithCredential(new Credential("viewer01", "I68VGU^nMurF", parent));
          });
}

UtilityNetworkTraceDemo::~UtilityNetworkTraceDemo() = default;

Map* UtilityNetworkTraceDemo::initMap_(QObject* parent) const
{
    return new Map(QUrl{"https://www.arcgis.com/home/item.html?id=471eb0bf37074b1fbb972b1da70fb310"}, parent);
}

Scene* UtilityNetworkTraceDemo::initScene_(QObject* /*parent*/) const
{
  return nullptr;
}
