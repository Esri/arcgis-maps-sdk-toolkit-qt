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
#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class AuthenticationViewDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  AuthenticationViewDemo(QObject* parent = nullptr);
  ~AuthenticationViewDemo();

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMO_H