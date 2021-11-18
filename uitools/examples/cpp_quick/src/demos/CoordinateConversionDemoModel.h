#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class CoordinateConversionDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE CoordinateConversionDemoModel(QObject* parent = nullptr);
  ~CoordinateConversionDemoModel() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMOMODEL_H
