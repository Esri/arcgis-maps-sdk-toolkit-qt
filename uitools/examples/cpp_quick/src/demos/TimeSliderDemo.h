#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class TimeSliderDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE TimeSliderDemo(QObject* parent = nullptr);
  ~TimeSliderDemo() override;

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMO_H
