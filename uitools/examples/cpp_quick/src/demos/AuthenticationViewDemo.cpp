// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
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
