#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class CoordinateConversionDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE CoordinateConversionDemo(QObject* parent = nullptr);
  ~CoordinateConversionDemo() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_COORDINATECONVERSIONDEMO_H
