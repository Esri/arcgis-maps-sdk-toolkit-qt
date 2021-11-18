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
