#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class AuthenticationViewDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  AuthenticationViewDemoModel(QObject* parent = nullptr);
  ~AuthenticationViewDemoModel();

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_AUTHENTICATIONVIEWDEMOMODEL_H
