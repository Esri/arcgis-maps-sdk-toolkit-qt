#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class NorthArrowDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE NorthArrowDemoModel(QObject* parent = nullptr);
  ~NorthArrowDemoModel() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMOMODEL_H
